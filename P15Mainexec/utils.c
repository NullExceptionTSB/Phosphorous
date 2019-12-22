#include "phosphorous.h"
#include <TlHelp32.h>

DWORD seed;

LPVOID CDECL lmemset(LPVOID dest, BYTE value, SIZE_T len) {
	for (INT i = 0; i < len; i++) ((BYTE*)dest)[i] = value;
	return dest;
}
LPVOID CDECL lmemcpy(LPVOID dest, LPVOID src, SIZE_T len) {
	for (INT i = 0; i < len; i++) ((BYTE*)dest)[i] = ((BYTE*)src)[i];
	return dest;
} 
LPVOID CDECL lmalloc(DWORD size) { return HeapAlloc(GetProcessHeap(), 0, size); }
LPVOID CDECL lcalloc(DWORD size) { return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size); }
LPVOID CDECL lfree(LPVOID mem) { return HeapFree(GetProcessHeap(), 0, mem); }
LPWSTR CDECL GenerateRandomNumericWString(DWORD dwLength) {
	LPWSTR lpRetString = (LPWSTR)lcalloc(dwLength * 2);
	for (INT i = 1; i < dwLength ; i ++) lpRetString[i] = 48 + (xorshift() % 11);
	return lpRetString;
}

DWORD xorshift() {
	DWORD x = seed;
	x ^= x << 13;	
	x ^= x >> 17;
	x ^= x << 5;
	return seed = x;
}

DWORD CDECL TextOutQ(HDC hDC, DWORD dwX, DWORD dwY, LPCWSTR lpMessage) {
	return TextOutW(hDC, dwX, dwY, lpMessage, lstrlenW(lpMessage));
}

VOID seedxorshift(DWORD _seed) {
	if (_seed != 0) seed = _seed;
}

VOID MakeBorderless(HWND hWindow) {
	RECT wndRect;
	GetWindowRect(hWindow, &wndRect);
	SetWindowLongW(hWindow, GWL_STYLE, GetWindowLongW(hWindow, GWL_STYLE) & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU));
	SetWindowLongW(hWindow, GWL_EXSTYLE, (GetWindowLongW(hWindow, GWL_EXSTYLE) & ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE)) | WS_EX_TOPMOST);
	SetWindowPos(hWindow, NULL, wndRect.left, wndRect.top, wndRect.right, wndRect.bottom, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
}

LONG MakeTransparrent(HWND hWindow) {
	return SetWindowLongW(hWindow, GWL_EXSTYLE, GetWindowLongW(hWindow, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
}

HWND MakeWindow(RECT Dimensions, LPCWSTR lpName) {
	return CreateWindowExW(0, L"STATIC", lpName, WS_VISIBLE, Dimensions.left, Dimensions.top, Dimensions.right - Dimensions.left, Dimensions.bottom - Dimensions.top, GetDesktopWindow(), NULL, GetModuleHandleW(NULL), NULL);
}

_inline RECT GetScreenSize() {
	RECT rcScreenSize;
	GetWindowRect(GetDesktopWindow(), &rcScreenSize);
	return rcScreenSize;
}

DWORD LookupProcessByName(LPCWSTR lpProcessName) {
	DWORD dwProcessID = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32FirstW(hSnapshot, &procEntry)) goto failed;

	for (;;) {
		if (!Process32NextW(hSnapshot, &procEntry)) goto failed;
		else if (!lstrcmpW(L"explorer.exe", procEntry.szExeFile)){ 
			dwProcessID = procEntry.th32ProcessID; 
			break;
		}
		else continue;
	}

	failed:
	if (!dwProcessID) SetLastError(ERROR_NOT_FOUND);
	return dwProcessID;
}

HANDLE mkthread(LPTHREAD_START_ROUTINE lpFunction, DWORD dwStartSuspended) { return CreateThread(NULL, 0, lpFunction, NULL, (dwStartSuspended * 4) & CREATE_SUSPENDED, NULL); }
HANDLE mkthreadp(LPTHREAD_START_ROUTINE lpFunction, DWORD dwStartSuspended, LPVOID lpParam) { return CreateThread(NULL, 0, lpFunction, lpParam, (dwStartSuspended * 4) & CREATE_SUSPENDED, NULL);}
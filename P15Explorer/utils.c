#include "p15explorer.h"

HANDLE hStdin = NULL;
HANDLE hStdout = NULL;

VOID CDECL WaitForConsoleInput() {
	UINT32 uRead;
	WCHAR cBuffer;
	if (hStdin == 0) hStdin = GetStdHandle(STD_INPUT_HANDLE);
	ReadConsoleW(hStdin, &cBuffer, 1, &uRead, NULL);
}


DWORD CDECL lputsW(LPCWSTR lpMessage) {
	UINT32 written;
	if (hStdout == 0) hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleW(hStdout, lpMessage, lstrlenW(lpMessage), &written, NULL);
	return written;
}

VOID CDECL lmemcpy(LPVOID lpDestination, LPCVOID lpSource, DWORD dwSize) {
	for (DWORD dwIndex = 0; dwIndex < dwSize; dwIndex++) ((CHAR*)lpDestination)[dwIndex] = ((const CHAR*)lpSource)[dwIndex];
}
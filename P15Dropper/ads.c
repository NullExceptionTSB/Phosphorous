#include "ads.h"

HANDLE CDECL PdpMoveFileFromADS(LPCWSTR lpStreamName, LPCWSTR lpPath, LPCWSTR lpStreamedPath) {
	HANDLE hStreamedFile = CreateFileW(lstrcatW(lpStreamedPath, lstrcat(L":", lpStreamName)), GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	HANDLE hOpenedFile = CreateFileW(lpPath, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
	DWORD dwFileSize = GetFileSize(hOpenedFile, NULL);
	LPVOID lpFileBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	LPOVERLAPPED lpOverlap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(OVERLAPPED));
	ReadFile(hStreamedFile, lpFileBuffer, dwFileSize, &dwFileSize, lpOverlap);
	WriteFile(hOpenedFile, lpFileBuffer, dwFileSize, &dwFileSize, NULL);
	CloseHandle(hStreamedFile);
	return hOpenedFile;
}

HANDLE CDECL PpdMoveFileToADS(LPCWSTR lpStreamName, LPCWSTR lpPath, LPCWSTR lpStreamedPath) {
	HANDLE hStreamedFile = CreateFileW(lstrcatW(lpStreamedPath, lstrcat(L":", lpStreamName)), GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	HANDLE hOpenedFile = CreateFileW(lpPath, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
	DWORD dwFileSize = GetFileSize(hOpenedFile, NULL);
	LPVOID lpFileBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	LPOVERLAPPED lpOverlap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(OVERLAPPED));
	ReadFile(hOpenedFile, lpFileBuffer, dwFileSize, &dwFileSize, lpOverlap);
	WriteFile(hStreamedFile, lpFileBuffer, dwFileSize, &dwFileSize, NULL);
	CloseHandle(hOpenedFile);
	return hStreamedFile;
}

HANDLE CDECL PpdExecuteFileFromADS(LPCWSTR lpStreamName, LPCWSTR lpPath) {
	LPSTARTUPINFOW lpStartupInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(STARTUPINFOW));
	LPPROCESS_INFORMATION lpProcessInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PROCESS_INFORMATION));
	lpStartupInfo->cb = sizeof(STARTUPINFOW);
	LPWSTR lpFullPath = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PATH);
	lstrcatW(lpFullPath, lpPath);
	lstrcatW(lpFullPath, L":");
	lstrcatW(lpFullPath, lpStreamName);
	return CreateProcessW(lpFullPath, NULL, NULL, NULL, 0, 0, NULL, NULL, lpStartupInfo, lpProcessInfo);	
}

DWORD CDECL PdpWriteDataToFileStream(LPCWSTR lpStreamName, LPCWSTR lpPath, LPVOID lpBuffer, DWORD dwBufferLen) {
	LPWSTR lpFullPath = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PATH);
	lstrcatW(lpFullPath, lpPath);
	lstrcatW(lpFullPath, L":");
	lstrcatW(lpFullPath, lpStreamName);
	HANDLE hStreamedFile = CreateFileW(lpFullPath, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
	DWORD dwWritten;
	WriteFile(hStreamedFile, lpBuffer, dwBufferLen, &dwWritten, NULL);
	CloseHandle(hStreamedFile);
	return dwWritten;
}

BYTE* CDECL PdpReadDataFromFileStream(LPCWSTR lpStreamName, LPCWSTR lpPath) {
	HANDLE hStreamedFile = CreateFileW(lstrcatW(lpPath, lstrcat(L":", lpStreamName)), GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	DWORD dwSize = GetFileSize(hStreamedFile, NULL);
	LPBYTE lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwSize);
	ReadFile(hStreamedFile, lpBuffer, dwSize, &dwSize, NULL);
	CloseHandle(hStreamedFile);
	return lpBuffer;
}

HANDLE CDECL PdpTouchADS(LPCWSTR lpStreamName, LPCWSTR lpPath) {
	LPWSTR lpFullPath = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PATH);
	lstrcatW(lpFullPath, lpPath);
	lstrcatW(lpFullPath, L":");
	lstrcatW(lpFullPath, lpStreamName);
	return CreateFileW(lpFullPath, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, 0, NULL);
}
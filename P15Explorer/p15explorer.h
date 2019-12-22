#include <Windows.h>

typedef NTSTATUS(*NtSetInformationProcessDef)(HANDLE, ULONG, PVOID, ULONG);
typedef NTSTATUS(*NtRaiseHardErrorDef)(DWORD, ULONG, ULONG, PULONG_PTR, ULONG, PULONG);
typedef NTSTATUS(*RtlAdjustPrivilegeDef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

EXTERN_C VOID CDECL WaitForConsoleInput();
EXTERN_C DWORD CDECL lputsW(LPCWSTR lpMessage);
EXTERN_C VOID CDECL lmemcpy(LPVOID lpDestination, LPCVOID lpSource, DWORD dwSize)

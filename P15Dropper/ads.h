#pragma once
#include <Windows.h>

EXTERN_C HANDLE CDECL PpdMoveFileToADS(LPCWSTR lpStreamName, LPCWSTR lpPath, LPCWSTR lpStreamedPath);
EXTERN_C HANDLE CDECL PdpMoveFileFromADS(LPCWSTR lpStreamName, LPCWSTR lpPath, LPCWSTR lpStreamedPath);
EXTERN_C HANDLE CDECL PpdExecuteFileFromADS(LPCWSTR lpStreamName, LPCWSTR lpPath);
EXTERN_C HANDLE CDECL PdpTouchADS(LPCWSTR lpStreamName, LPCWSTR lpPath);
EXTERN_C DWORD CDECL PdpWriteDataToFileStream(LPCWSTR lpStreamName, LPCWSTR lpPath, LPVOID lpBuffer, DWORD dwBufferLen);
EXTERN_C BYTE* CDECL PdpReadDataFromFileStream(LPCWSTR lpStreamName, LPCWSTR lpPath);
EXTERN_C HANDLE CDECL PdpTouchADS(LPCWSTR lpStreamName, LPCWSTR lpPath);
#pragma once
#include <objbase.h>
#include <Windows.h>
#include <wincred.h>

extern decltype(ChangeWindowMessageFilter)* Orig_ChangeWindowMessageFilter;
extern decltype(ChangeWindowMessageFilterEx)* Orig_ChangeWindowMessageFilterEx;
extern decltype(SendMessageA)* Orig_SendMessageA;
extern decltype(SendMessageW)* Orig_SendMessageW;
extern decltype(GetMessageA)* Orig_GetMessageA;
extern decltype(GetMessageW)* Orig_GetMessageW;
extern decltype(PeekMessageA)* Orig_PeekMessageA;
extern decltype(PeekMessageW)* Orig_PeekMessageW;
extern decltype(CreateFileW)* Orig_CreateFileW;
extern decltype(ReadFile)* Orig_ReadFile;
extern decltype(CredReadA)* Orig_CredReadA;
extern decltype(CredReadW)* Orig_CredReadW;
extern decltype(CoCreateInstance)* Orig_CoCreateInstance;
extern decltype(OpenProcessToken)* Orig_OpenProcessToken;
extern decltype(OpenThreadToken)* Orig_OpenThreadToken;


LRESULT WINAPI Hook_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Hook_SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI Hook_ChangeWindowMessageFilter(UINT message, DWORD dwFlag);
BOOL WINAPI Hook_ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct);
BOOL WINAPI Hook_GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL WINAPI Hook_GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL WINAPI Hook_PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL WINAPI Hook_PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
HANDLE WINAPI Hook_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL WINAPI Hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
BOOL WINAPI Hook_CredReadW (LPCWSTR TargetName, DWORD Type, DWORD Flags, PCREDENTIALW *Credential);
BOOL WINAPI Hook_CredReadA (LPCSTR TargetName, DWORD Type, DWORD Flags, PCREDENTIALA *Credential);
HRESULT STDAPICALLTYPE Hook_CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID  FAR * ppv);
BOOL WINAPI Hook_OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);


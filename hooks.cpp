#include "pch.h"
#include "hooks.h"
#include "util.h"

decltype(ChangeWindowMessageFilter)* Orig_ChangeWindowMessageFilter = ::ChangeWindowMessageFilter;
decltype(ChangeWindowMessageFilterEx)* Orig_ChangeWindowMessageFilterEx = ::ChangeWindowMessageFilterEx;
decltype(SendMessageA)* Orig_SendMessageA = ::SendMessageA;
decltype(SendMessageW)* Orig_SendMessageW = ::SendMessageW;
decltype(GetMessageA)* Orig_GetMessageA = ::GetMessageA;
decltype(GetMessageW)* Orig_GetMessageW = ::GetMessageW;
decltype(PeekMessageA)* Orig_PeekMessageA = ::PeekMessageA;
decltype(PeekMessageW)* Orig_PeekMessageW = ::PeekMessageW;
decltype(CreateFileW)* Orig_CreateFileW = ::CreateFileW;
decltype(ReadFile)* Orig_ReadFile = ::ReadFile;
decltype(CredReadA)* Orig_CredReadA = ::CredReadA;
decltype(CredReadW)* Orig_CredReadW = ::CredReadW;
decltype(CoCreateInstance)* Orig_CoCreateInstance = ::CoCreateInstance;
decltype(OpenProcessToken)* Orig_OpenProcessToken = ::OpenProcessToken;
decltype(OpenThreadToken)* Orig_OpenThreadToken = ::OpenThreadToken;
decltype(DuplicateToken)* Orig_DuplicateToken = ::DuplicateToken;
decltype(DuplicateTokenEx)* Orig_DuplicateTokenEx = ::DuplicateTokenEx;
decltype(WTSQueryUserToken)* Orig_WTSQueryUserToken = ::WTSQueryUserToken;
decltype(CreateProcessAsUserW)* Orig_CreateProcessAsUserW = ::CreateProcessAsUserW;

LRESULT WINAPI Hook_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT l = Orig_SendMessageA(hWnd, Msg, wParam, lParam);
    LogFormat("Hook_SendMessageA %x %d", hWnd, Msg);
    return l;
}

LRESULT WINAPI Hook_SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT l = Orig_SendMessageW(hWnd, Msg, wParam, lParam);
    LogFormat("Hook_SendMessageW %x %d", hWnd, Msg);
    return l;
}

BOOL WINAPI Hook_ChangeWindowMessageFilter(UINT message, DWORD dwFlag)
{
    BOOL result = Orig_ChangeWindowMessageFilter(message, dwFlag);
    LogFormat("Hook_ChangeWindowMessageFilter %d %x", message, dwFlag);
    return result;
}

BOOL WINAPI Hook_ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct)
{
    BOOL result = Orig_ChangeWindowMessageFilterEx(hwnd, message, action, pChangeFilterStruct);
    LogFormat("Hook_ChangeWindowMessageFilterEx %x %d %x", hwnd, message, action);
    return result;
}

BOOL WINAPI Hook_GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
    BOOL result = Orig_GetMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
    LogFormat("Hook_GetMessageA %d %x %x", result, hWnd, lpMsg ? lpMsg->message : 0);
    return result;
}

BOOL WINAPI Hook_GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
    BOOL result = Orig_GetMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
    LogFormat("Hook_GetMessageW %d %x %x", result, hWnd, lpMsg ? lpMsg->message : 0);
    return result;
}

BOOL WINAPI Hook_PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
    BOOL result = Orig_PeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
    LogFormat("Hook_PeekMessageA %d %x %x", result, hWnd, lpMsg ? lpMsg->message : 0);
    return result;
}

BOOL WINAPI Hook_PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
    BOOL result = Orig_PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
    LogFormat("Hook_PeekMessageW %d %x %x", result, hWnd, lpMsg ? lpMsg->message : 0);
    return result;
}

HANDLE WINAPI Hook_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    HANDLE h = Orig_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    LogFormat("Hook_CreateFileW %S", lpFileName, h);
    return h;
}

BOOL WINAPI Hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    BOOL result = Orig_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    LogFormat("Hook_ReadFile %x", hFile);
    return result;
}

BOOL WINAPI Hook_CredReadW (LPCWSTR TargetName, DWORD Type, DWORD Flags, PCREDENTIALW *Credential)
{
    BOOL result = Orig_CredReadW(TargetName, Type, Flags, Credential);
    LogFormat("Hook_CredReadW %S", TargetName);
    return result;
}

BOOL WINAPI Hook_CredReadA (LPCSTR TargetName, DWORD Type, DWORD Flags, PCREDENTIALA *Credential)
{
    BOOL result = Orig_CredReadA(TargetName, Type, Flags, Credential);
    LogFormat("Hook_CredReadA %s", TargetName);
    return result;
}

HRESULT STDAPICALLTYPE Hook_CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID  FAR * ppv)
{
    HRESULT result = Orig_CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
    const auto clsidFriendlyName = GetFriendlyNameForCLSID(rclsid);
    LogFormat("Hook_CoCreateInstance %S, result: 0x%0X", clsidFriendlyName, result);
    return result;
}

BOOL WINAPI Hook_OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle)
{
    BOOL result = Orig_OpenProcessToken(ProcessHandle, DesiredAccess, TokenHandle);
    DWORD gle = result ? 0 : GetLastError();
    LogFormat("Hook_OpenProcessToken %08X %08X %08X %d %d", ProcessHandle, DesiredAccess, TokenHandle ? *TokenHandle : 0, result, gle);
    return result;
}

BOOL WINAPI Hook_OpenThreadToken(HANDLE ThreadHandle, DWORD DesiredAccess, BOOL OpenAsSelf, PHANDLE TokenHandle)
{
    BOOL result = Orig_OpenThreadToken(ThreadHandle, DesiredAccess, OpenAsSelf, TokenHandle);
    LogFormat("Hook_OpenProcessToken %08X %08X %d %08X %d", ThreadHandle, DesiredAccess, OpenAsSelf, TokenHandle ? *TokenHandle : 0, result);
    return result;
}

BOOL WINAPI Hook_DuplicateToken(HANDLE ExistingTokenHandle, SECURITY_IMPERSONATION_LEVEL ImpersonationLevel, PHANDLE DuplicateTokenHandle)
{
    BOOL result = Orig_DuplicateToken(ExistingTokenHandle, ImpersonationLevel, DuplicateTokenHandle);
    LogFormat("Hook_DuplicateToken %08X %d %08X %d", ExistingTokenHandle, ImpersonationLevel, DuplicateTokenHandle, result);
    return result;
}

BOOL WINAPI Hook_DuplicateTokenEx(HANDLE hExistingToken, DWORD dwDesiredAccess, LPSECURITY_ATTRIBUTES lpTokenAttributes, SECURITY_IMPERSONATION_LEVEL ImpersonationLevel, TOKEN_TYPE TokenType, PHANDLE phNewToken)
{
    /*
    char buffer[2048] = { 0 };
    DWORD bufferLen = 2048;
    for( int tokenInfo = 1; tokenInfo <= MaxTokenInfoClass; ++tokenInfo )
    {
        DWORD resultLen = bufferLen;
        BOOL result = GetTokenInformation(hExistingToken, (TOKEN_INFORMATION_CLASS)tokenInfo, buffer, bufferLen, &resultLen);
        if( result )
        {
            std::string resultDump = hexDump(buffer, min(resultLen, 512), resultLen);
            LogFormat("TokenInfo %d %d bytes %s", tokenInfo, resultLen, resultDump.c_str());
        }
    }
    */

    BOOL result = Orig_DuplicateTokenEx(hExistingToken, dwDesiredAccess, lpTokenAttributes, ImpersonationLevel, TokenType, phNewToken);
    DWORD gle = result ? 0 : GetLastError();
    LogFormat("Hook_DuplicateTokenEx %08X %08X %d %d %08X %d %d", hExistingToken, dwDesiredAccess, ImpersonationLevel, TokenType, phNewToken ? *phNewToken : 0, result, gle);
    return result;
}

BOOL WINAPI Hook_WTSQueryUserToken(ULONG SessionId, PHANDLE phToken)
{
    BOOL result = Orig_WTSQueryUserToken(SessionId, phToken);
    LogFormat("Hook_WTSQueryUserToken %d %08X %d", SessionId, phToken ? *phToken : 0, result);
    return result;
}

BOOL WINAPI Hook_CreateProcessAsUserW(HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
    BOOL result = Orig_CreateProcessAsUserW(hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    LogFormat("Hook_CreateProcessAsUserW %08X %S %S %d %08X %S %d", hToken, lpApplicationName, lpCommandLine, bInheritHandles, dwCreationFlags, lpCurrentDirectory, result);
    return result;
}


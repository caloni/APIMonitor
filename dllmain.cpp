/* API Monitor

Writes API calls to a log file using Detour lib and
a hard coded list of APIs.

@author  Wanderley Caloni <wanderley.caloni@gmail.com>
@date 2025-04
*/
#include "pch.h"
#include "hooks.h"
#include "util.h"

#include <detours.h>

#include <combaseapi.h>
#include <strsafe.h>
#include <wincred.h>
#include <windows.h>

#include <mutex>
#include <vector>

using namespace std;

struct Hook
{
    PVOID& original;
    PVOID hook;
    bool enabled;
};

static std::mutex g_logLock{};
static bool g_startMessage = false;

static Hook g_hooks[] =
{
    { (PVOID&)Orig_ChangeWindowMessageFilter, Hook_ChangeWindowMessageFilter, false },
    { (PVOID&)Orig_ChangeWindowMessageFilterEx, Hook_ChangeWindowMessageFilterEx, false },
    { (PVOID&)Orig_SendMessageA, Hook_SendMessageA, false },
    { (PVOID&)Orig_SendMessageW, Hook_SendMessageW, false },
    { (PVOID&)Orig_SendMessageW, Hook_SendMessageW, false },
    { (PVOID&)Orig_GetMessageA, Hook_GetMessageA, false },
    { (PVOID&)Orig_GetMessageW, Hook_GetMessageW, false },
    { (PVOID&)Orig_PeekMessageA, Hook_PeekMessageA, false },
    { (PVOID&)Orig_PeekMessageW, Hook_PeekMessageW, false },
    { (PVOID&)Orig_CreateFileW, Hook_CreateFileW, false },
    { (PVOID&)Orig_ReadFile, Hook_ReadFile, false },
    { (PVOID&)Orig_CredReadA, Hook_CredReadA, false },
    { (PVOID&)Orig_CredReadW, Hook_CredReadW, false },
    { (PVOID&)Orig_CoCreateInstance, Hook_CoCreateInstance, false },
    { (PVOID&)Orig_OpenProcessToken, Hook_OpenProcessToken, true },
};

static void InstallHooks()
{
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    for (auto& hook : g_hooks)
    {
        if (!hook.enabled)
        {
            continue;
        }
        DetourAttach(&(PVOID&)hook.original, hook.hook);
    }
    DetourTransactionCommit();
}

static void UninstallHooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    for (auto& hook : g_hooks)
    {
        if (!hook.enabled)
        {
            continue;
        }
        DetourDetach(&hook.original, hook.hook);
    }
    DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        if (g_startMessage)
        {
            MessageBoxA(NULL, "Ready to install hooks", "Dll1", 0);
        }
        LogFormat("Installing hooks");
        InstallHooks();
        LogFormat("Hooks installed");
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        LogFormat("Uninstalling hooks");
        UninstallHooks();
        LogFormat("Hooks uninstalled");
    }
    return TRUE;
}


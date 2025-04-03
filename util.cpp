#include "pch.h"
#include "hooks.h"
#include "util.h"

static std::wstring g_hookFileName = L"C:\\temp\\hook.log";

std::string GetLocalTimeStamp()
{
    static constexpr size_t timestamp_size = 23; // "YYYY-MM-DD hh:mm:ss.fff"
    char timeStamp[timestamp_size + 1]; // +1 for null terminator
    
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf_s(timeStamp, sizeof(timeStamp),
        "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    return std::string(timeStamp, timestamp_size);
}

const char* GetProcessName()
{
    static char st_processName[MAX_PATH]{};
    if( !*st_processName )
    {
        GetModuleFileNameA(NULL, st_processName, MAX_PATH);
        if( char* slash = strrchr(st_processName, '\\') )
        {
            memmove(st_processName, slash + 1, strlen(slash));
        }
    }
    return st_processName;
}

void LogWrite(const char* str)
{
    HANDLE h = Orig_CreateFileW(g_hookFileName.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if( h != INVALID_HANDLE_VALUE )
    {
        char buf[500];
        //sprintf_s(buf, "%s [%d:%d] [%s] %s\r\n", GetLocalTimeStamp().c_str(), static_cast<int>(GetCurrentProcessId()), static_cast<int>(GetCurrentThreadId()), GetProcessName(), str);
        sprintf_s(buf, "[%s] %s\r\n", GetProcessName(), str);
        DWORD bufLen = (DWORD) strlen(buf);
        DWORD written = 0;
        if( WriteFile(h, buf, bufLen, &written, NULL) && bufLen == written )
        {
            SetFilePointer(h, 0, NULL, FILE_END);
        }
        CloseHandle(h);
    }
}

std::wstring GUIDToWString(const GUID* guid)
{
  wchar_t guid_string[37] = {};
  swprintf_s(
      guid_string, _countof(guid_string),
      L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
      guid->Data1, guid->Data2, guid->Data3,
      guid->Data4[0], guid->Data4[1], guid->Data4[2],
      guid->Data4[3], guid->Data4[4], guid->Data4[5],
      guid->Data4[6], guid->Data4[7]);
  return std::wstring(guid_string);
}

bool GetCorrectRegistryKey(const std::wstring& keyPath, HKEY* key)
{
    auto hResult = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_READ, key);
    if (hResult == ERROR_SUCCESS)
    {
        return true;
    }

    hResult = ::RegOpenKeyExW(HKEY_CLASSES_ROOT, keyPath.c_str(), 0, KEY_READ, key);
    if (hResult == ERROR_SUCCESS)
    {
        return true;
    }

    hResult = ::RegOpenKeyExW(HKEY_CLASSES_ROOT, keyPath.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, key);
    if (hResult == ERROR_SUCCESS)
    {
        return true;
    }

    // Failed to open CLSID reg key
    return false;
}

LSTATUS ReadStringValue(HKEY key, const std::wstring& valueName, std::wstring& value)
{
    DWORD valueType = REG_NONE;
    std::vector<wchar_t> valueData(0x400 * 32); //  max path is 32KB for long paths
    auto valueSize = static_cast<DWORD>(valueData.size() * sizeof(wchar_t));

    auto res = ::RegQueryValueExW(key, valueName.empty() ? nullptr : valueName.c_str(), nullptr, &valueType, reinterpret_cast<LPBYTE>(valueData.data()), &valueSize);
    if (res == ERROR_SUCCESS)
    {
        value = std::wstring{ valueData.data() };
    }

    return res;
}

std::wstring GetFriendlyNameForCLSID(REFIID clsid)
{
    auto clsidStr = GUIDToWString(&clsid);
    auto keyPath = L"CLSID\\{" + clsidStr + L"}";

    auto friendlyName = clsidStr;
    if (HKEY key = nullptr; GetCorrectRegistryKey(keyPath, &key))
    {
        ReadStringValue(key, {}, friendlyName);
    }

    return friendlyName;
}

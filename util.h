#pragma once
#include <windows.h>
#include <strsafe.h>
#include <string>
#include <vector>

std::string GetLocalTimeStamp();
const char* GetProcessName();
void LogWrite(const char* str);
std::wstring GUIDToWString(const GUID* guid);
bool GetCorrectRegistryKey(const std::wstring& keyPath, HKEY* key);
LSTATUS ReadStringValue(HKEY key, const std::wstring& valueName, std::wstring& value);
std::wstring GetFriendlyNameForCLSID(REFIID clsid);
std::string hexDump(const char* data, size_t size, size_t lineWidth = 16);

template <typename... Args>
void LogFormat(const char* format, Args... args)
{
    HRESULT hr;
    std::vector<char> buf(4096);
    do
    {
        hr = StringCbPrintfA(buf.data(), buf.size() * sizeof(char), format, args...);
        if (hr == S_OK)
        {
            break;
        }
        if (hr == STRSAFE_E_INVALID_PARAMETER)
        {
            return;
        }
        buf.resize(buf.size() * 2);

    } while (hr == STRSAFE_E_INSUFFICIENT_BUFFER);

    LogWrite(buf.data());
}


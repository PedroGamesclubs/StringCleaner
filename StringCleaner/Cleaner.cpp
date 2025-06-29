#include "Cleaner.hpp"
#include "MemoryScanner.hpp"
#include "StringUtils.hpp"
#include "ProcessManager.hpp"
#include <windows.h>
std::map<std::wstring, std::vector<std::string>> ProcessesToClean = {

    //Process
    { L"lsass.exe", { "skript.gg" } }


};

std::map<std::wstring, std::vector<std::string>> ServicesToClean = {

    //Services
    { L"DPS", { "!!WinRAR.exe!2025/03/20:10:00:20!336cbb!" } }
};

namespace Cleaner {

    void ClearStrings(DWORD pid, const std::vector<std::string>& entries) {
        HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (!handle) return;

        for (const auto& str : entries) {
            auto asciiMatches = MemoryScanner::FindAsciiStringAddresses(pid, str);
            for (auto addr : asciiMatches) {
                std::string nulls(str.size(), '\0');
                WriteProcessMemory(handle, addr, nulls.c_str(), str.size(), nullptr);
            }

            auto wideStr = StringUtils::ToWideString(str);
            auto unicodeMatches = MemoryScanner::FindUnicodeStringAddresses(pid, wideStr);
            for (auto addr : unicodeMatches) {
                std::wstring nulls(wideStr.size(), L'\0');
                WriteProcessMemory(handle, addr, nulls.c_str(), wideStr.size() * sizeof(wchar_t), nullptr);
            }
        }

        CloseHandle(handle);
    }

    void Run() {
        for (const auto& [name, list] : ProcessesToClean) {
            DWORD pid = ProcessManager::GetProcessIdByName(name);
            if (pid) ClearStrings(pid, list);
        }

        for (const auto& [service, list] : ServicesToClean) {
            DWORD pid = ProcessManager::GetProcessIdByService(service);
            if (pid) ClearStrings(pid, list);
        }
    }

}

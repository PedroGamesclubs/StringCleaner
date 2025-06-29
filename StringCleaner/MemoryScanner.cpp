#include "MemoryScanner.hpp"
#include <algorithm>

namespace MemoryScanner {

    std::vector<LPVOID> FindAsciiStringAddresses(DWORD pid, const std::string& target) {
        std::vector<LPVOID> results;
        HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (!handle) return results;

        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        LPVOID base = sysInfo.lpMinimumApplicationAddress;
        LPVOID max = sysInfo.lpMaximumApplicationAddress;

        std::string pattern = target;
        std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);

        while (base < max) {
            MEMORY_BASIC_INFORMATION mbi;
            if (VirtualQueryEx(handle, base, &mbi, sizeof(mbi)) == 0) break;

            if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS && mbi.Protect != PAGE_GUARD) {
                std::vector<char> buffer(mbi.RegionSize);
                SIZE_T read;
                if (ReadProcessMemory(handle, mbi.BaseAddress, buffer.data(), buffer.size(), &read)) {
                    std::string content(buffer.begin(), buffer.end());
                    std::transform(content.begin(), content.end(), content.begin(), ::tolower);
                    size_t pos = content.find(pattern);
                    while (pos != std::string::npos) {
                        results.push_back(reinterpret_cast<LPVOID>((DWORD_PTR)mbi.BaseAddress + pos));
                        pos = content.find(pattern, pos + 1);
                    }
                }
            }

            base = reinterpret_cast<LPVOID>((DWORD_PTR)base + mbi.RegionSize);
        }

        CloseHandle(handle);
        return results;
    }

    std::vector<LPVOID> FindUnicodeStringAddresses(DWORD pid, const std::wstring& target) {
        std::vector<LPVOID> results;
        HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (!handle) return results;

        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        LPVOID base = sysInfo.lpMinimumApplicationAddress;
        LPVOID max = sysInfo.lpMaximumApplicationAddress;

        std::wstring pattern = target;
        std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::towlower);

        while (base < max) {
            MEMORY_BASIC_INFORMATION mbi;
            if (VirtualQueryEx(handle, base, &mbi, sizeof(mbi)) == 0) break;

            if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS && mbi.Protect != PAGE_GUARD) {
                std::vector<wchar_t> buffer(mbi.RegionSize / sizeof(wchar_t));
                SIZE_T read;
                if (ReadProcessMemory(handle, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &read)) {
                    std::wstring content(buffer.begin(), buffer.end());
                    std::transform(content.begin(), content.end(), content.begin(), ::towlower);
                    size_t pos = content.find(pattern);
                    while (pos != std::wstring::npos) {
                        results.push_back(reinterpret_cast<LPVOID>((DWORD_PTR)mbi.BaseAddress + pos * sizeof(wchar_t)));
                        pos = content.find(pattern, pos + 1);
                    }
                }
            }

            base = reinterpret_cast<LPVOID>((DWORD_PTR)base + mbi.RegionSize);
        }

        CloseHandle(handle);
        return results;
    }

} 

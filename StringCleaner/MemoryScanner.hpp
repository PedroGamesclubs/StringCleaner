#pragma once
#include <windows.h>
#include <vector>
#include <string>

namespace MemoryScanner {
    std::vector<LPVOID> FindAsciiStringAddresses(DWORD pid, const std::string& target);
    std::vector<LPVOID> FindUnicodeStringAddresses(DWORD pid, const std::wstring& target);
}
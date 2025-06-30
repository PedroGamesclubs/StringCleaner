#include "StringUtil.hpp"
#include <locale>
#include <codecvt>
#include <windows.h>
#include <string>

namespace StringUtils {
    std::wstring ToWideString(const std::string& input) {
        if (input.empty()) return std::wstring();

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), (int)input.size(), nullptr, 0);
        std::wstring result(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, input.c_str(), (int)input.size(), &result[0], size_needed);
        return result;
    }
}


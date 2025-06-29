#include "StringUtils.hpp"
#include <locale>
#include <codecvt>

namespace StringUtils {
    std::wstring ToWideString(const std::string& input) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(input);
    }
}

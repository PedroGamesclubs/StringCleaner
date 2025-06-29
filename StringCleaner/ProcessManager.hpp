#pragma once
#include <string>
#include <windows.h>

namespace ProcessManager {
    DWORD GetProcessIdByName(const std::wstring& name);
    DWORD GetProcessIdByService(const std::wstring& service);
}

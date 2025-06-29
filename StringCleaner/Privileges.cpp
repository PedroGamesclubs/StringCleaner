#include "Privileges.hpp"

namespace Privileges {
    bool EnableDebugPrivilege() {
        HANDLE token;
        TOKEN_PRIVILEGES priv;
        LUID luid;

        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) return false;
        if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid)) {
            CloseHandle(token);
            return false;
        }

        priv.PrivilegeCount = 1;
        priv.Privileges[0].Luid = luid;
        priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        bool result = AdjustTokenPrivileges(token, FALSE, &priv, sizeof(priv), nullptr, nullptr);
        CloseHandle(token);
        return result;
    }
}

#include "ProcessManager.hpp"
#include <tlhelp32.h>
#include <winsvc.h>

namespace ProcessManager {

    DWORD GetProcessIdByName(const std::wstring& name) {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) return 0;

        PROCESSENTRY32W entry;
        entry.dwSize = sizeof(entry);

        if (Process32FirstW(snap, &entry)) {
            do {
                if (name == entry.szExeFile) {
                    CloseHandle(snap);
                    return entry.th32ProcessID;
                }
            } while (Process32NextW(snap, &entry));
        }

        CloseHandle(snap);
        return 0;
    }

    DWORD GetProcessIdByService(const std::wstring& service) {
        SC_HANDLE scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
        if (!scm) return 0;

        int size = WideCharToMultiByte(CP_ACP, 0, service.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string serviceA(size, 0);
        WideCharToMultiByte(CP_ACP, 0, service.c_str(), -1, &serviceA[0], size, nullptr, nullptr);

        SC_HANDLE handle = OpenServiceA(scm, serviceA.c_str(), SERVICE_QUERY_STATUS);
        if (!handle) {
            CloseServiceHandle(scm);
            return 0;
        }

        SERVICE_STATUS_PROCESS status;
        DWORD needed;
        if (!QueryServiceStatusEx(handle, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&status), sizeof(status), &needed)) {
            CloseServiceHandle(handle);
            CloseServiceHandle(scm);
            return 0;
        }

        CloseServiceHandle(handle);
        CloseServiceHandle(scm);
        return status.dwProcessId;
    }

}

#include "Privileges.hpp"
#include "Cleaner.hpp"
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING


int main() {
    Privileges::EnableDebugPrivilege();
    Cleaner::Run();
    return 0;
}

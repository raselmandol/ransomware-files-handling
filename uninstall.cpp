#include <iostream>
#include <Windows.h>

int main() {
    //define the program name you want to uninstall
    const wchar_t* programName = L"ProgramName";

    //use the Windows API to launch the uninstaller
    int result = static_cast<int>(ShellExecuteW(nullptr, L"runas", L"control", L"appwiz.cpl", nullptr, SW_SHOWNORMAL));

    //check if the ShellExecuteW function succeeded or failed
    if (result <= 32) {
        std::cerr << "failed to execute the control panel applet (appwiz.cpl)." << std::endl;
        return 1;
    }

    //give the control panel applet some time to open
    Sleep(2000);

    //use the Windows API to search for the uninstallation window and close it
    HWND uninstallWindow = FindWindowW(L"#32770", L"Programs and Features");

    if (uninstallWindow) {
        HWND programEntry = FindWindowExW(uninstallWindow, nullptr, L"SysListView32", nullptr);

        if (programEntry) {
            SendMessageW(programEntry, LVM_SETITEMSTATE, 0, LVIS_SELECTED | LVIS_FOCUSED);
            SendMessageW(uninstallWindow, WM_COMMAND, 0x0006E, 0);
        }
    }

    return 0;
}

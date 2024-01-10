#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

//listing background tasks
void ListBackgroundTasks() {
    SC_HANDLE scmHandle = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
    if (!scmHandle) {
        std::cerr << "Failed to open the services control manager. Error: " << GetLastError() << std::endl;
        return;
    }
  
    DWORD servicesCount;
    DWORD bytesNeeded;
    EnumServicesStatusEx(
        scmHandle,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        nullptr,
        0,
        &bytesNeeded,
        &servicesCount,
        nullptr,
        nullptr
    );

    if (GetLastError() != ERROR_MORE_DATA) {
        std::cerr << "Failed to get service information. Error: " << GetLastError() << std::endl;
        CloseServiceHandle(scmHandle);
        return;
    }

    LPENUM_SERVICE_STATUS_PROCESS servicesBuffer = (LPENUM_SERVICE_STATUS_PROCESS)LocalAlloc(LPTR, bytesNeeded);
    if (!servicesBuffer) {
        std::cerr << "Failed to allocate memory for service information. Error: " << GetLastError() << std::endl;
        CloseServiceHandle(scmHandle);
        return;
    }

    if (!EnumServicesStatusEx(
        scmHandle,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        reinterpret_cast<LPBYTE>(servicesBuffer),
        bytesNeeded,
        &bytesNeeded,
        &servicesCount,
        nullptr,
        nullptr
    )) {
        std::cerr << "Failed to enumerate services. Error: " << GetLastError() << std::endl;
        LocalFree(servicesBuffer);
        CloseServiceHandle(scmHandle);
        return;
    }

    std::cout << "List of background tasks/services running:" << std::endl;
    for (DWORD i = 0; i < servicesCount; ++i) {
        std::wcout << "Service Name: " << servicesBuffer[i].lpServiceName << std::endl;
        std::wcout << "Display Name: " << servicesBuffer[i].lpDisplayName << std::endl;
        std::wcout << "Service Status: " << (servicesBuffer[i].ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING ? "Running" : "Stopped") << std::endl;
        std::wcout << "-------------------------------------------------------" << std::endl;
    }

    LocalFree(servicesBuffer);
    CloseServiceHandle(scmHandle);
}

int main() {
    ListBackgroundTasks();
    return 0;
}

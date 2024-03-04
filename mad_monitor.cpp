#include <iostream>
#include <windows.h>

void sendEmailAlert(const std::string& message) {
    //sending an email alert goes here
  //add your own configs and codes here
//add mail, smtp
    std::cout << "Email alert sent: " << message << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <path>" << std::endl;
        return 1;
    }

    const std::string path = argv[1];
    std::cout << "monitoring changes to: " << path << std::endl;

    //HANDLE dir
    HANDLE dir = CreateFile(
        path.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (dir == INVALID_HANDLE_VALUE) {
        std::cerr << "error opening directory--> " << path << std::endl;
        return 1;
    }

    char buffer[1024];
    DWORD bytesReturned;
    FILE_NOTIFY_INFORMATION* fileInfo;

    //directorychangesW--?:f
    while (true) {
        if (!ReadDirectoryChangesW(
            dir,
            buffer,
            sizeof(buffer),
            TRUE,
            FILE_NOTIFY_CHANGE_LAST_WRITE,
            &bytesReturned,
            NULL,
            NULL
        )) {
            std::cerr << "error reading directory changes." << std::endl;
            break;
        }

        fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);

        while (true) {
            //filename_fileInfo
            std::wstring fileName(fileInfo->FileName, fileInfo->FileNameLength / sizeof(WCHAR));

            if (fileName == L"" || fileName == L"." || fileName == L"..") {
                //ignoring the . and .. directory entries
            }
            else if (fileInfo->Action == FILE_ACTION_MODIFIED) {
                //sending an alert if the file was modified
                const std::string message = "File modified--> " + std::string(fileName.begin(), fileName.end());
                sendEmailAlert(message);
            }

            if (fileInfo->NextEntryOffset == 0) {
                break;
            }

            fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer + fileInfo->NextEntryOffset);
        }
    }

    CloseHandle(dir);

    return 0;
}

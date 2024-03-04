#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

//add_more_extensions
std::vector<std::string> ransomwareExtensions = {
    ".locky",
    ".crypt",
    ".cerber",
};
//add more extensions

void scanForRansomware(const std::string& path) {
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            for (const auto& ext : ransomwareExtensions) {
                if (entry.path().extension() == ext) {
                    std::cout << "Potential ransomware file found: " << entry.path() << std::endl;
                    //do whatever you want
                   
                }
            }
        }
    }
}

int main() {

     //replace "C:\\" with the root directory you want to scan
    std::string rootPath = "C:\\";
    
    try {
        if (fs::exists(rootPath) && fs::is_directory(rootPath)) {
            scanForRansomware(rootPath);
        } else {
            std::cerr << "Invalid directory: " << rootPath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

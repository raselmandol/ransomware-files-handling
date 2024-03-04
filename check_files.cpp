#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

using namespace std;
namespace fs = filesystem;
//add more ransomware_extensions as you can to check
vector<string> ransomware_extensions = {".locky", ".zepto", ".osiris", ".odin", ".nemucod", ".cryptolocker", ".cryptowall", ".maktub"};

bool is_ransomware_extension(string filename) {
    for (auto ext : ransomware_extensions) {
        if (filename.find(ext) != string::npos) {
            return true;
        }
    }
    return false;
}

void search_directory(fs::path directory) {
    for (auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry)) {
            //search_directory
            search_directory(entry);
        } else {
            string filename = entry.path().filename().string();
            //checking ext, if yes, entry path
            if (is_ransomware_extension(filename)) {
                cout << "Encrypted file: " << entry.path() << endl;
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cout << "Usage: " << argv[0] << " [directory1] [directory2] [directory3]" << endl;
        return 0;
    }
    //edit path directory
    fs::path directory1(argv[1]);
    if (!fs::is_directory(directory1)) {
        cout << "Error: " << argv[1] << " is not a directory." << endl;
    } else {
        search_directory(directory1);
    }
    fs::path directory2(argv[2]);
    if (!fs::is_directory(directory2)) {
        cout << "Error: " << argv[2] << " is not a directory." << endl;
    } else {
        search_directory(directory2);
    }
    fs::path directory3(argv[3]);
    //check_directory
    if (!fs::is_directory(directory3)) {
        cout << "Error: " << argv[3] << " is not a directory." << endl;
    } else {
        search_directory(directory3);
    }
    return 0;
}

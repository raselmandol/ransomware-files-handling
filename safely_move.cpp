#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

//checking if a file has a ransomware extension
//you can add more extensions here
//extensions-->update
bool isRansomware(const std::string& fileName) {
    static const std::vector<std::string> extensions = {
        ".locky", ".crypt", ".cryptolocker", ".zepto", ".odin", ".osiris", ".cerber",
        ".dharma", ".lechiffre", ".maktub", ".xtbl", ".crypren", ".cryptowall", ".crysis",
        ".cryakl", ".globe", ".shade", ".7ev3n", ".nemucod", ".nm4", ".hydracrypt", ".lukitus",
        ".thor", ".necurs", ".vesad", ".snatch", ".etols", ".mamba", ".dxxd", ".mole", ".trumplocker",
        ".rapid", ".gandcrab", ".sodinokibi", ".evopro", ".stop", ".mosk", ".coot", ".mogera",
        ".buran", ".brrr", ".redrum", ".medusa", ".access", ".vorasto", ".shinigami", ".fox",
        ".masodas", ".lokas", ".pysa", ".tycoon", ".mount", ".npsk", ".lockbit", ".rikud", ".roger",
        ".pay2me", ".kraken", ".sunshine", ".blackmatter", ".brute", ".hakbit", ".xpx", ".reha",
        ".aeolus", ".efji", ".crimson", ".megacortex", ".zeppelin", ".suncrypt", ".darkside",
        ".babuk", ".contifio", ".ragnarlocker", ".prolock", ".eternalpetya", ".fuxsocy", ".snake",
        ".tycoon", ".lockbit2"
    };
    for (const auto& ext : extensions) {
        if (fileName.size() > ext.size() &&
            fileName.compare(fileName.size() - ext.size(), ext.size(), ext) == 0) {
            return true;
        }
    }
    return false;
}

int main() {
    std::string sourceFolder = "C:/"; //replace with your desired source folder/drive
    std::string destFolder = "C:/encrypted_files/"; //replace with your desired destination folder

    //source folder checking
    for (const auto& entry : fs::recursive_directory_iterator(sourceFolder)) {
        if (fs::is_regular_file(entry) && isRansomware(entry.path().extension().string())) {
            //moving file to destination folder
            std::string destFilePath = destFolder + entry.path().filename().string();
            fs::rename(entry, destFilePath);
            std::cout << "Moved " << entry.path() << " to " << destFilePath << std::endl;
        }
    }
    return 0;
}

#include <iostream>
#include <fstream>
#include <string>

bool isFileEncryptedWithAES(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (file) {
        char header[16];
        file.read(header, sizeof(header));
        //for AES, it could be something like checking for "AES-128" or "AES-256"
        bool isAESSignature = /*check if header matches AES signature*/;
        
        file.close();
        return isAESSignature;
    }
    return false;
}

int main() {
    //filePath-->set here
    std::string filePath = "example.txt";
    if (isFileEncryptedWithAES(filePath)) {
        std::cout << "File is encrypted with AES.\n";
    } else {
        std::cout << "File is not encrypted with AES.\n";
    }
    return 0;
}

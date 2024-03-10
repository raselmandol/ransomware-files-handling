#include <iostream>
#include <fstream>
#include <Windows.h>
#include <wincrypt.h>

void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    //check_error
    if (!inputFile) {
        std::cerr << "Error: Failed to open the input file." << std::endl;
        return;
    }
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Failed to create the output file." << std::endl;
        return;
    }
    HCRYPTPROV hCryptProv = NULL;
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "Error: CryptAcquireContext failed." << std::endl;
        return;
    }

    const int ivSize = 16;
    BYTE iv[ivSize];
    if (!CryptGenRandom(hCryptProv, ivSize, iv)) {
        std::cerr << "Error: CryptGenRandom failed." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return;
    }

    const int keySize = 32; 
    BYTE key[keySize];
    if (!CryptDeriveKey(hCryptProv, CALG_AES_256, NULL, CRYPT_CREATE_SALT, reinterpret_cast<const BYTE*>(password.c_str()), password.size(), key)) {
        std::cerr << "Error: CryptDeriveKey failed." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return;
    }
    outputFile.write(reinterpret_cast<const char*>(iv), ivSize);

    HCRYPTKEY hKey = NULL;
    if (!CryptImportKey(hCryptProv, key, keySize, NULL, 0, &hKey)) {
        std::cerr << "Error: CryptImportKey failed." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return;
    }

    const int bufferSize = 4096;
    BYTE buffer[bufferSize];
    DWORD bytesRead, cipherTextLength;
    while (inputFile.read(reinterpret_cast<char*>(buffer), bufferSize)) {
        bytesRead = static_cast<DWORD>(inputFile.gcount());
        if (!CryptEncrypt(hKey, NULL, TRUE, 0, buffer, &bytesRead, bufferSize)) {
            std::cerr << "Error: CryptEncrypt failed." << std::endl;
            CryptDestroyKey(hKey);
            CryptReleaseContext(hCryptProv, 0);
            return;
        }
        outputFile.write(reinterpret_cast<const char*>(buffer), bytesRead);
    }

    CryptDestroyKey(hKey);
    CryptReleaseContext(hCryptProv, 0);

    inputFile.close();
    outputFile.close();

    std::cout << "File encrypted successfully." << std::endl;
}

int main() {
    std::string inputFilePath = "input.txt";  
    std::string outputFilePath = "output.enc";  
    std::string password = "my_secret_password";

    encryptFile(inputFilePath, outputFilePath, password);

    return 0;
}

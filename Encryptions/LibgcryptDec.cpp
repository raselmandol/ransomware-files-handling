#include <iostream>
#include <fstream>
#include <cstring>
#include <gcrypt.h>

void decryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
    gcry_control(GCRYCTL_DISABLE_SECMEM, 0);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
    const int ivSize = 16;
    unsigned char iv[ivSize];
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile.read(reinterpret_cast<char*>(iv), ivSize)) {
        std::cerr << "Error: Failed to read IV from input file." << std::endl;
        return;
    }

    const int keySize = 32;
    unsigned char key[keySize];
    gcry_kdf_derive(password.c_str(), password.size(), GCRY_KDF_PBKDF2, GCRY_MD_SHA256, nullptr, 0, 4096, keySize, key);

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Failed to create the output file." << std::endl;
        return;
    }

    gcry_cipher_hd_t cipherHandle;
    gcry_cipher_open(&cipherHandle, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_GCM, 0);
    gcry_cipher_setkey(cipherHandle, key, keySize);
    gcry_cipher_setiv(cipherHandle, iv, ivSize);
    //buffer
    const int bufferSize = 4096;
    unsigned char buffer[bufferSize];
    while (inputFile.read(reinterpret_cast<char*>(buffer), bufferSize)) {
        int bytesRead = static_cast<int>(inputFile.gcount());
        gcry_cipher_decrypt(cipherHandle, buffer, bytesRead, nullptr, 0);
        outputFile.write(reinterpret_cast<const char*>(buffer), bytesRead);
    }

    gcry_cipher_close(cipherHandle);
    //close file
    inputFile.close();
    outputFile.close();

    std::cout << "File decrypted successfully." << std::endl;
}

int main() {
    //set inputPath, password
    std::string inputFilePath = "output.enc"; 
    std::string outputFilePath = "decrypted.txt";
    std::string password = "my_secret_password";
    decryptFile(inputFilePath, outputFilePath, password);

    return 0;
}

#include <iostream>
#include <fstream>
#include <openssl/evp.h>
bool decryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Failed to open the input file." << std::endl;
        return false;
    }

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Failed to create the output file." << std::endl;
        return false;
    }

    unsigned char iv[EVP_MAX_IV_LENGTH];
    inputFile.read(reinterpret_cast<char*>(iv), EVP_MAX_IV_LENGTH);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char*>(password.c_str()), iv);
    const int bufferSize = 4096;
    unsigned char buffer[bufferSize];
    int bytesRead, plainTextLength;
    while ((bytesRead = inputFile.read(reinterpret_cast<char*>(buffer), bufferSize).gcount()) > 0) {
        if (EVP_DecryptUpdate(ctx, buffer, &plainTextLength, buffer, bytesRead) != 1) {
            std::cerr << "Error: Failed to decrypt data." << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        outputFile.write(reinterpret_cast<const char*>(buffer), plainTextLength);
    }

    if (EVP_DecryptFinal_ex(ctx, buffer, &plainTextLength) != 1) {
        std::cerr << "Error: Failed to finalize decryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    outputFile.write(reinterpret_cast<const char*>(buffer), plainTextLength);
    EVP_CIPHER_CTX_free(ctx);
    inputFile.close();
    outputFile.close();

    std::cout << "File decrypted successfully." << std::endl;
    return true;
}

int main() {
    std::string inputFilePath = "output.enc";
    std::string outputFilePath = "decrypted.txt";
    std::string password = "my_secret_password";
    if (decryptFile(inputFilePath, outputFilePath, password)) {
        std::cout << "Decryption completed successfully." << std::endl;
    } else {
        std::cerr << "Decryption failed." << std::endl;
    }

    return 0;
}

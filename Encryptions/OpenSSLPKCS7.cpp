#include <iostream>
#include <fstream>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/rand.h>

void handleErrors() {
    std::cerr << "Error: OpenSSL operation failed." << std::endl;
    ERR_print_errors_fp(stderr);
    throw std::runtime_error("OpenSSL error");
}
void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
    // Initialize OpenSSL library
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    unsigned char iv[EVP_MAX_IV_LENGTH];
    if (RAND_bytes(iv, EVP_MAX_IV_LENGTH) != 1) {
        handleErrors();
        return;
    }
    unsigned char key[EVP_MAX_KEY_LENGTH];
    if (EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), nullptr,
                       reinterpret_cast<const unsigned char*>(password.c_str()), password.length(), 1, key, iv) != 32) {
        handleErrors();
        return;
    }
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Failed to open the input file." << std::endl;
        return;
    }
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Failed to create the output file." << std::endl;
        return;
    }
    outputFile.write(reinterpret_cast<const char*>(iv), EVP_MAX_IV_LENGTH);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
        return;
    }
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    const int bufferSize = 4096;
    unsigned char buffer[bufferSize];
    int bytesRead, cipherTextLength;
    while ((bytesRead = inputFile.read(reinterpret_cast<char*>(buffer), bufferSize).gcount()) > 0) {
        if (!EVP_EncryptUpdate(ctx, buffer, &cipherTextLength, buffer, bytesRead)) {
            handleErrors();
            EVP_CIPHER_CTX_free(ctx);
            return;
        }
        outputFile.write(reinterpret_cast<const char*>(buffer), cipherTextLength);
    }
    if (!EVP_EncryptFinal_ex(ctx, buffer, &cipherTextLength)) {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    outputFile.write(reinterpret_cast<const char*>(buffer), cipherTextLength);
    EVP_CIPHER_CTX_free(ctx);
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

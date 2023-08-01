#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/rand.h>
bool encryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
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
    if (RAND_bytes(iv, EVP_MAX_IV_LENGTH) != 1) {
        std::cerr << "Error: Failed to generate IV." << std::endl;
        return false;
    }
    outputFile.write(reinterpret_cast<const char*>(iv), EVP_MAX_IV_LENGTH);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char*>(password.c_str()), iv);
    const int bufferSize = 4096;
    unsigned char buffer[bufferSize];
    int bytesRead, cipherTextLength;
    while ((bytesRead = inputFile.read(reinterpret_cast<char*>(buffer), bufferSize).gcount()) > 0) {
        if (EVP_EncryptUpdate(ctx, buffer, &cipherTextLength, buffer, bytesRead) != 1) {
            std::cerr << "Error: Failed to encrypt data." << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        outputFile.write(reinterpret_cast<const char*>(buffer), cipherTextLength);
    }
    if (EVP_EncryptFinal_ex(ctx, buffer, &cipherTextLength) != 1) {
        std::cerr << "Error: Failed to finalize encryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    outputFile.write(reinterpret_cast<const char*>(buffer), cipherTextLength);
    EVP_CIPHER_CTX_free(ctx);
    inputFile.close();
    outputFile.close();
    std::cout << "File encrypted successfully." << std::endl;
    return true;
}
int main() {
    std::string inputFilePath = "input.txt";    //Replace with the path to your input file
    std::string outputFilePath = "output.enc";   //Replace with the desired path for the encrypted output file
    std::string password = "my_secret_password";  //Replace with your desired password

    if (encryptFile(inputFilePath, outputFilePath, password)) {
        std::cout << "Encryption completed successfully." << std::endl;
    } else {
        std::cerr << "Encryption failed." << std::endl;
    }
    return 0;
}

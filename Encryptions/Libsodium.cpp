#include <iostream>
#include <fstream>
#include <sodium.h>

void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
    if (sodium_init() < 0) {
        std::cerr << "Error: Libsodium initialization failed." << std::endl;
        return;
    }
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes(nonce, sizeof nonce);
    unsigned char key[crypto_secretbox_KEYBYTES];
    crypto_secretbox_keygen(key);

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

    outputFile.write(reinterpret_cast<const char*>(nonce), sizeof nonce);

    char buffer[4096];
    while (inputFile.read(buffer, sizeof buffer)) {
        unsigned char cipherText[sizeof buffer + crypto_secretbox_MACBYTES];
        crypto_secretbox_easy(cipherText, reinterpret_cast<const unsigned char*>(buffer), sizeof buffer, nonce, key);
        outputFile.write(reinterpret_cast<const char*>(cipherText), sizeof cipherText);
    }
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

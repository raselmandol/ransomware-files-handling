#include <iostream>
#include <fstream>
#include <sodium.h>

void decryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
    //checking initialization 
    if (sodium_init() < 0) {
        std::cerr << "Error: Libsodium initialization failed." << std::endl;
        return;
    }

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile.read(reinterpret_cast<char*>(nonce), sizeof nonce)) {
        std::cerr << "Error: Failed to read nonce from input file." << std::endl;
        return;
    }
    //key_in
    unsigned char key[crypto_secretbox_KEYBYTES];
    crypto_secretbox_keygen(key);

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Failed to create the output file." << std::endl;
        return;
    }
    //buffer
    char buffer[4096];
    while (inputFile.read(buffer, sizeof buffer)) {
        unsigned char plainText[sizeof buffer - crypto_secretbox_MACBYTES];
        if (crypto_secretbox_open_easy(plainText, reinterpret_cast<const unsigned char*>(buffer), sizeof buffer, nonce, key) != 0) {
            std::cerr << "Error: Failed to decrypt data." << std::endl;
            return;
        }
        outputFile.write(reinterpret_cast<const char*>(plainText), sizeof plainText);
    }

    inputFile.close();
    outputFile.close();
    std::cout << "File decrypted successfully." << std::endl;
}

int main() {
    std::string inputFilePath = "output.enc"; 
    std::string outputFilePath = "decrypted.txt";
    std::string password = "my_secret_password";

    decryptFile(inputFilePath, outputFilePath, password);

    return 0;
}

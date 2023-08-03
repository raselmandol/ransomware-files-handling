#include <iostream>
#include <fstream>
#include <botan/botan.h>
#include <botan/aes.h>
#include <botan/cipher_mode.h>
#include <botan/filters.h>
bool decryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
    Botan::LibraryInitializer init;

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
    Botan::InitializationVector iv(Botan::InitializationVector::create(inputFile, Botan::AES::BLOCK_SIZE));
    Botan::SymmetricKey key(password);
    std::unique_ptr<Botan::Cipher_Mode> cipher(Botan::Cipher_Mode::create("AES-256/CBC/PKCS7", Botan::DECRYPTION));
    cipher->set_key(key);
    cipher->start(iv.bits_of());
    const size_t bufferSize = 4096;
    char buffer[bufferSize];
    while (inputFile.read(buffer, bufferSize)) {
        size_t bytesRead = static_cast<size_t>(inputFile.gcount());
        cipher->update(reinterpret_cast<uint8_t*>(buffer), bytesRead, reinterpret_cast<uint8_t*>(buffer));
        outputFile.write(buffer, bytesRead);
    }
    cipher->finish(buffer, 0);
    outputFile.write(buffer, cipher->output_length(0));
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

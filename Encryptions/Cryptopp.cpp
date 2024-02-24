#include <iostream>
#include <fstream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
bool encryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
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
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(iv, iv.size());
    outputFile.write(reinterpret_cast<const char*>(iv.BytePtr()), iv.size());
    CryptoPP::AES::Encryption aesEncryption(reinterpret_cast<const byte*>(password.data()), CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
    const int bufferSize = 4096;
    char buffer[bufferSize];
    while (inputFile.read(buffer, bufferSize)) {
        int bytesRead = static_cast<int>(inputFile.gcount());
        CryptoPP::ArraySource(buffer, bytesRead, true,
            new CryptoPP::StreamTransformationFilter(cbcEncryption,
                new CryptoPP::FileSink(outputFile)));
    }
    inputFile.close();
    outputFile.close();

    std::cout << "File encrypted successfully." << std::endl;
    return true;
}
int main() {
    std::string inputFilePath = "input.txt";  //input file
    std::string outputFilePath = "output.enc";  //Replace with the desired path for the encrypted output file
    std::string password = "your_secret_password"; //Replace with your desired password

    if (encryptFile(inputFilePath, outputFilePath, password)) {
        std::cout << "Encryption completed successfully." << std::endl;
    } else {
        std::cerr << "Encryption failed." << std::endl;
    }
    return 0;
}

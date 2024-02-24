#include <iostream>
#include <fstream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

bool decryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& password) {
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
    inputFile.read(reinterpret_cast<char*>(iv.BytePtr()), iv.size());
    CryptoPP::AES::Decryption aesDecryption(reinterpret_cast<const byte*>(password.data()), CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);
    const int bufferSize = 4096;
    char buffer[bufferSize];
    while (inputFile.read(buffer, bufferSize)) {
        int bytesRead = static_cast<int>(inputFile.gcount());
        CryptoPP::ArraySource(buffer, bytesRead, true,
            new CryptoPP::StreamTransformationFilter(cbcDecryption,
                new CryptoPP::FileSink(outputFile)));
    }
    inputFile.close();
    outputFile.close();

    std::cout << "File decrypted successfully." << std::endl;
    return true;
}

int main() {
    //input_file path
    std::string inputFilePath = "output.enc";
    std::string outputFilePath = "decrypted.txt";
    //set pass here
    std::string password = "my_secret_password"; 

    if (decryptFile(inputFilePath, outputFilePath, password)) {
        std::cout << "Decryption completed successfully." << std::endl;
    } else {
        std::cerr << "Decryption failed." << std::endl;
    }

    return 0;
}

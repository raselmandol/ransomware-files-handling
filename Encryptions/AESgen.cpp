#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <openssl/aes.h>

void encryptAES(const std::string& inputFile, const std::string& outputFile, const std::string& key) {
    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);

    AES_KEY aesKey;
    AES_set_encrypt_key(reinterpret_cast<const unsigned char*>(key.c_str()), key.size() * 8, &aesKey);

    const int blockSize = AES_BLOCK_SIZE;
    unsigned char input[blockSize];
    unsigned char output[blockSize];

    while (in.read(reinterpret_cast<char*>(input), blockSize)) {
        AES_encrypt(input, output, &aesKey);
        out.write(reinterpret_cast<const char*>(output), blockSize);
    }

    in.close();
    out.close();
}

int main() {
    std::string inputFile = "input.txt";
    std::string encryptedFile = "output.fun";
    std::string key = "mysecretkey123";

    encryptAES(inputFile, encryptedFile, key);

    std::cout << "File encrypted successfully." << std::endl;

    return 0;
}

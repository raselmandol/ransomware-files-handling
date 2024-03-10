#include <iostream>
#include <string>
#include <cryptopp/twofish.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

int main() {
    try {
        //set key
        const std::string key = "mySecretKey";
        const std::string plaintext = "Hello, Twofish!";
        byte keyBytes[CryptoPP::Twofish::DEFAULT_KEYLENGTH];
        std::memcpy(keyBytes, key.c_str(), CryptoPP::Twofish::DEFAULT_KEYLENGTH);

        byte iv[CryptoPP::Twofish::BLOCKSIZE];
        CryptoPP::OS_GenerateRandomBlock(false, iv, sizeof(iv));
        
        byte ciphertext[plaintext.size()];
        CryptoPP::CBC_Mode<CryptoPP::Twofish>::Encryption encryptor(keyBytes, CryptoPP::Twofish::DEFAULT_KEYLENGTH, iv);

        encryptor.ProcessData(ciphertext, (const byte*)plaintext.data(), plaintext.size());
        byte decryptedText[plaintext.size()];
        CryptoPP::CBC_Mode<CryptoPP::Twofish>::Decryption decryptor(keyBytes, CryptoPP::Twofish::DEFAULT_KEYLENGTH, iv);
        decryptor.ProcessData(decryptedText, ciphertext, plaintext.size());

        std::cout << "Plaintext: " << plaintext << std::endl;
        std::cout << "Ciphertext: ";
        for (size_t i = 0; i < plaintext.size(); ++i) {
            printf("%02X", ciphertext[i]);
        }
        std::cout << std::endl;
        std::cout << "Decrypted Text: " << decryptedText << std::endl;

    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Crypto++ Exception: " << e.what() << std::endl;
    }
    return 0;
}

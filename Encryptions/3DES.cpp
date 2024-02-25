#include <iostream>
#include <string>
#include <cryptopp/des.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

int main() {
    try {
        //set_password
        const std::string key = "mySecretKey12345678";
        const std::string plaintext = "Hello, 3DES!";
        byte keyBytes[CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH];
        std::memcpy(keyBytes, key.c_str(), CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH);
        byte iv[CryptoPP::DES_EDE3::BLOCKSIZE];
        CryptoPP::OS_GenerateRandomBlock(false, iv, sizeof(iv));
        byte ciphertext[plaintext.size()];
        CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Encryption encryptor(keyBytes, CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH, iv);
        encryptor.ProcessData(ciphertext, (const byte*)plaintext.data(), plaintext.size());

        byte decryptedText[plaintext.size()];
        CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Decryption decryptor(keyBytes, CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH, iv);
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

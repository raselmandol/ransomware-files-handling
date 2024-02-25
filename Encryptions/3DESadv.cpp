#include <iostream>
#include <cryptopp/des.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

int main() {
    try {
        const byte key[CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH] = {
            0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
            0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
            0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67
        };
        const byte iv[CryptoPP::DES_EDE3::BLOCKSIZE] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
        };
        CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(
            CryptoPP::DES_EDE3::Encryption(key, CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH),
            iv
        );

        CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(
            CryptoPP::DES_EDE3::Decryption(key, CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH),
            iv
        );
        //set_test
        std::string plaintext = "Hello, Triple DES!";
        std::string ciphertext;
        CryptoPP::StringSource(plaintext, true,
            new CryptoPP::StreamTransformationFilter(cbcEncryption,
                new CryptoPP::StringSink(ciphertext)
            )
        );

        std::cout << "Encrypted: " << ciphertext << std::endl;
        std::string decrypted;
        CryptoPP::StringSource(ciphertext, true,
            new CryptoPP::StreamTransformationFilter(cbcDecryption,
                new CryptoPP::StringSink(decrypted)
            )
        );

        std::cout << "Decrypted: " << decrypted << std::endl;
    } catch (const CryptoPP::Exception &ex) {
        std::cerr << "Crypto++ Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

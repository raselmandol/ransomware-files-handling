#include <iostream>
//use cryptopp
#include <cryptopp/twofish.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

int main() {
    try {
        CryptoPP::Twofish::Encryption twofishEncryption;
        CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(twofishEncryption, new CryptoPP::StreamTransformationFilter::BlockPaddingSchemeDef::PKCS_PADDING);
        byte key[CryptoPP::Twofish::DEFAULT_KEYLENGTH];
        byte iv[CryptoPP::Twofish::BLOCKSIZE];
        std::memset(key, 0x00, CryptoPP::Twofish::DEFAULT_KEYLENGTH);
        std::memset(iv, 0x00, CryptoPP::Twofish::BLOCKSIZE);

        twofishEncryption.SetKeyWithIV(key, CryptoPP::Twofish::DEFAULT_KEYLENGTH, iv);
        std::string plaintext = "Hello, Twofish!";
        std::string ciphertext;
        CryptoPP::StringSource(plaintext, true,
            new CryptoPP::StreamTransformationFilter(cbcEncryption,
                new CryptoPP::StringSink(ciphertext)
            )
        );

        std::cout << "Encrypted: " << ciphertext << std::endl;
        CryptoPP::Twofish::Decryption twofishDecryption;
        CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(twofishDecryption, new CryptoPP::StreamTransformationFilter::BlockPaddingSchemeDef::PKCS_PADDING);
        twofishDecryption.SetKeyWithIV(key, CryptoPP::Twofish::DEFAULT_KEYLENGTH, iv);
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

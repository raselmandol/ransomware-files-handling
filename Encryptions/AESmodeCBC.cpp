#include <iostream>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

int main() {
    try {
        const std::string key = "mySecretAESKey1234";
        const std::string iv = "initializationVect";
        const std::string plaintext = "Hello, AES!";
        byte keyBytes[CryptoPP::AES::DEFAULT_KEYLENGTH];
        std::memcpy(keyBytes, key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
        byte ivBytes[CryptoPP::AES::BLOCKSIZE];
        std::memcpy(ivBytes, iv.c_str(), CryptoPP::AES::BLOCKSIZE);
        byte ciphertext[plaintext.size()];
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor(keyBytes, CryptoPP::AES::DEFAULT_KEYLENGTH, ivBytes);
        CryptoPP::ArraySink cs(ciphertext, sizeof(ciphertext));
        CryptoPP::StreamTransformationFilter stf(encryptor, new CryptoPP::Redirector(cs));
        stf.Put(reinterpret_cast<const byte*>(plaintext.data()), plaintext.size());
        stf.MessageEnd();
        byte decryptedText[plaintext.size()];
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor(keyBytes, CryptoPP::AES::DEFAULT_KEYLENGTH, ivBytes);
        CryptoPP::ArraySink ds(decryptedText, sizeof(decryptedText));
        CryptoPP::StreamTransformationFilter ddf(decryptor, new CryptoPP::Redirector(ds));
        ddf.Put(ciphertext, sizeof(ciphertext));
        ddf.MessageEnd();

        std::cout << "Plaintext: " << plaintext << std::endl;
        std::cout << "Ciphertext: ";
        for (size_t i = 0; i < sizeof(ciphertext); ++i) {
            printf("%02X", ciphertext[i]);
        }
        std::cout << std::endl;
        std::cout << "Decrypted Text: " << decryptedText << std::endl;
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Crypto++ Exception: " << e.what() << std::endl;
    }
    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

class RSAWrapper {
public:
    RSAWrapper(int bits) : rsa(RSA_new(), ::RSA_free) {
        BIGNUM *e = BN_new();
        BN_set_word(e, RSA_F4);
        RSA_generate_key_ex(rsa.get(), bits, e, nullptr);
        BN_free(e);
    }

    RSA *get() const {
        return rsa.get();
    }

private:
    std::unique_ptr<RSA, decltype(&::RSA_free)> rsa;
};

class RSAEncryption {
public:
//passing_key
    RSAEncryption(RSA *key) : rsa(key) {}

    std::vector<unsigned char> encrypt(const std::vector<unsigned char> &data) {
        std::vector<unsigned char> encrypted;
        int keySize = RSA_size(rsa);

        int blocks = (data.size() + keySize - 1) / keySize;

        for (int i = 0; i < blocks; ++i) {
            std::vector<unsigned char> chunk(std::min(keySize, static_cast<int>(data.size()) - i * keySize));
            std::copy(data.begin() + i * keySize, data.begin() + (i + 1) * keySize, chunk.begin());

            std::vector<unsigned char> encryptedChunk(keySize);
            int encryptedSize = RSA_public_encrypt(chunk.size(), chunk.data(), encryptedChunk.data(), rsa, RSA_PKCS1_PADDING);
            if (encryptedSize == -1) {
                handleErrors();
            }
            encrypted.insert(encrypted.end(), encryptedChunk.begin(), encryptedChunk.begin() + encryptedSize);
        }

        return encrypted;
    }

    std::vector<unsigned char> decrypt(const std::vector<unsigned char> &encryptedData) {
        std::vector<unsigned char> decrypted;
        int keySize = RSA_size(rsa);

        int blocks = (encryptedData.size() + keySize - 1) / keySize;

        for (int i = 0; i < blocks; ++i) {
            std::vector<unsigned char> chunk(std::min(keySize, static_cast<int>(encryptedData.size()) - i * keySize));
            std::copy(encryptedData.begin() + i * keySize, encryptedData.begin() + (i + 1) * keySize, chunk.begin());

            std::vector<unsigned char> decryptedChunk(keySize);
            int decryptedSize = RSA_private_decrypt(chunk.size(), chunk.data(), decryptedChunk.data(), rsa, RSA_PKCS1_PADDING);
            if (decryptedSize == -1) {
                handleErrors();
            }
            decrypted.insert(decrypted.end(), decryptedChunk.begin(), decryptedChunk.begin() + decryptedSize);
        }

        return decrypted;
    }

private:
    void handleErrors() {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    RSA *rsa;
};

int main() {
    try {
        RSAWrapper rsaWrapper(2048);
        RSAEncryption rsaEncryption(rsaWrapper.get());
        const char *plaintext = "Hello, RSA!";
        std::vector<unsigned char> plaintextData(plaintext, plaintext + strlen(plaintext));
        std::vector<unsigned char> encryptedData = rsaEncryption.encrypt(plaintextData);
        std::cout << "Encrypted data size: " << encryptedData.size() << " bytes" << std::endl;
        std::vector<unsigned char> decryptedData = rsaEncryption.decrypt(encryptedData);
        std::string decryptedText(decryptedData.begin(), decryptedData.end());
        std::cout << "Decrypted text: " << decryptedText << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

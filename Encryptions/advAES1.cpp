#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <memory>
//add openssl_aes
#include <openssl/aes.h>
#include <openssl/rand.h>

class AESCipher {
public:
    AESCipher(const unsigned char *key, const unsigned char *iv) {
        aesKey = std::make_unique<unsigned char[]>(AES_BLOCK_SIZE);
        aesIv = std::make_unique<unsigned char[]>(AES_BLOCK_SIZE);
        std::memcpy(aesKey.get(), key, AES_BLOCK_SIZE);
        std::memcpy(aesIv.get(), iv, AES_BLOCK_SIZE);
        AES_set_encrypt_key(aesKey.get(), 128, &encKey);
        AES_set_decrypt_key(aesKey.get(), 128, &decKey);
    }
    std::vector<unsigned char> encrypt(const std::vector<unsigned char> &data) {
        std::vector<unsigned char> encrypted(data.size());
        AES_cbc_encrypt(data.data(), encrypted.data(), data.size(), &encKey, aesIv.get(), AES_ENCRYPT);
        return encrypted;
    }

    std::vector<unsigned char> decrypt(const std::vector<unsigned char> &encryptedData) {
        std::vector<unsigned char> decrypted(encryptedData.size());
        AES_cbc_encrypt(encryptedData.data(), decrypted.data(), encryptedData.size(), &decKey, aesIv.get(), AES_DECRYPT);
        return decrypted;
    }

private:
    std::unique_ptr<unsigned char[]> aesKey;
    std::unique_ptr<unsigned char[]> aesIv;

    AES_KEY encKey, decKey;
};
int main() {
    const unsigned char aesKey[AES_BLOCK_SIZE] = "mysecretaeskey12";
    unsigned char aesIv[AES_BLOCK_SIZE];
    RAND_bytes(aesIv, AES_BLOCK_SIZE);

    try {
        AESCipher aesCipher(aesKey, aesIv);
        const char *plaintext = "Hello, AES!";
        std::vector<unsigned char> plaintextData(plaintext, plaintext + strlen(plaintext));
        std::vector<unsigned char> encryptedData = aesCipher.encrypt(plaintextData);
        std::cout << "Encrypted data size: " << encryptedData.size() << " bytes" << std::endl;
        std::vector<unsigned char> decryptedData = aesCipher.decrypt(encryptedData);
        std::string decryptedText(decryptedData.begin(), decryptedData.end());
        std::cout << "Decrypted text: " << decryptedText << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

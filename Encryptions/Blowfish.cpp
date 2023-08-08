#include <iostream>
#include <cstring>
#include <openssl/blowfish.h>

void encryptBlowfish(const unsigned char *plaintext, int plaintextLength, const unsigned char *key, unsigned char *ciphertext) {
    BF_KEY bfKey;
    BF_set_key(&bfKey, strlen((const char *)key), key);
    BF_ecb_encrypt(plaintext, ciphertext, &bfKey, BF_ENCRYPT);
}

void decryptBlowfish(const unsigned char *ciphertext, int ciphertextLength, const unsigned char *key, unsigned char *plaintext) {
    BF_KEY bfKey;
    BF_set_key(&bfKey, strlen((const char *)key), key);
    BF_ecb_encrypt(ciphertext, plaintext, &bfKey, BF_DECRYPT);
}

int main() {
    const unsigned char key[] = "mySecretKey";
    const unsigned char plaintext[] = "Hello, Blowfish!";
    unsigned char ciphertext[sizeof(plaintext)];
    unsigned char decryptedText[sizeof(plaintext)];

    encryptBlowfish(plaintext, sizeof(plaintext), key, ciphertext);
    decryptBlowfish(ciphertext, sizeof(ciphertext), key, decryptedText);

    std::cout << "Plaintext: " << plaintext << std::endl;
    std::cout << "Ciphertext: ";
    for (int i = 0; i < sizeof(ciphertext); ++i) {
        printf("%02X", ciphertext[i]);
    }
    std::cout << std::endl;
    std::cout << "Decrypted Text: " << decryptedText << std::endl;

    return 0;
}

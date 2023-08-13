#include <iostream>
#include <cryptopp/des.h>

int main() {
    try {
        const char *keyStr1 = "Rasel";
        const char *keyStr2 = "Mandol";
        const char *keyStr3 = "Nothing";

        CryptoPP::SecByteBlock key(CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH);
        for (size_t i = 0; i < CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH; ++i) {
            key[i] = static_cast<byte>(keyStr1[i % strlen(keyStr1)]) ^
                     static_cast<byte>(keyStr2[i % strlen(keyStr2)]) ^
                     static_cast<byte>(keyStr3[i % strlen(keyStr3)]);
        }

        std::cout << "Derived key: ";
        for (size_t i = 0; i < key.size(); ++i) {
            std::cout << std::hex << static_cast<int>(key[i]);
        }
        std::cout << std::endl;

    } catch (const CryptoPP::Exception &ex) {
        std::cerr << "Crypto++ Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

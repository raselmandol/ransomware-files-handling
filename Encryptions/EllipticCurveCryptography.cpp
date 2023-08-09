#include <iostream>
#include <string>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>

int main() {
    using namespace CryptoPP;
    AutoSeededRandomPool prng;
    ECIES<ECP>::PrivateKey privateKey;
    privateKey.Initialize(prng, ASN1::secp256r1());
    ECIES<ECP>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);
    std::string plaintext = "Hello, ECC!";
    std::string ciphertext;
    ECIES<ECP>::Encryptor encryptor(publicKey);
    StringSource(plaintext, true,
                 new PK_EncryptorFilter(prng, encryptor,
                                        new StringSink(ciphertext)));
    std::string decryptedText;

    ECIES<ECP>::Decryptor decryptor(privateKey);
    StringSource(ciphertext, true,
                 new PK_DecryptorFilter(prng, decryptor,
                                        new StringSink(decryptedText)));
    std::cout << "Plaintext: " << plaintext << std::endl;
    std::cout << "Ciphertext: " << ciphertext << std::endl;
    std::cout << "Decrypted Text: " << decryptedText << std::endl;

    return 0;
}

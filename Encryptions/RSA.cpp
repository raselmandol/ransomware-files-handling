#include <iostream>
#include <fstream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

void handleErrors()
{
    ERR_print_errors_fp(stderr);
    exit(1);
}
int main()
{
    const char *publicKeyFile = "public_key.pem";
    FILE *pubKeyFile = fopen(publicKeyFile, "rb");
    if (!pubKeyFile)
    {
        std::cerr << "Error: Could not open public key file." << std::endl;
        return 1;
    }
    RSA *rsaPublicKey = PEM_read_RSA_PUBKEY(pubKeyFile, NULL, NULL, NULL);
    fclose(pubKeyFile);
    if (!rsaPublicKey)
    {
        std::cerr << "Error: Could not read public key." << std::endl;
        handleErrors();
    }
    const char *inputFileName = "input.txt";
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile)
    {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }
    const char *outputFileName = "encrypted.bin";
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile)
    {
        std::cerr << "Error: Could not open output file." << std::endl;
        return 1;
    }
    int keySizeBytes = RSA_size(rsaPublicKey);
    unsigned char inputBuffer[keySizeBytes];
    unsigned char encryptedBuffer[keySizeBytes];
    int bytesRead;
    while ((bytesRead = inputFile.read((char *)inputBuffer, keySizeBytes)))
    {
        int encryptedSize = RSA_public_encrypt(bytesRead, inputBuffer, encryptedBuffer, rsaPublicKey, RSA_PKCS1_PADDING);
        if (encryptedSize == -1)
        {
            std::cerr << "Error: RSA encryption failed." << std::endl;
            handleErrors();
        }
        outputFile.write((char *)encryptedBuffer, encryptedSize);
    }
    std::cout << "File encrypted successfully." << std::endl;
    RSA_free(rsaPublicKey);
    inputFile.close();
    outputFile.close();

    return 0;
}

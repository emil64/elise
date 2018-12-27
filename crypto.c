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

struct Params{
    int a;
    unsigned char *key;
    FILE *plaintext;
    FILE *out;
    unsigned char *IV;
    bool encrypt;
    const EVP_CIPHER *cipher;
};

typedef struct Params Params;

void toCipher(Params *p){

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
    FILE *Out = fopen(in, "rb");
    FILE *In = fopen(out, "wb");
    char message[2000];
    strcpy(message, "rm ");
    strcat(message, in);

    p->plaintext = In;
    p->out = Out;

    toCipher(p);

    system(message);
    fclose(In);
    fclose(Out);

    free(p);
}

void decrypt(FILE *in, uint8_t *key){

}

int main(){
    encryptAES("out", (unsigned char *) "1234567889sjjda");
}

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

int public_encrypt(unsigned char *text, int len, unsigned char * key, unsigned char *encrypted){

    RSA *rsa = createRSA(key, true);
    int res =  RSA_public_encrypt(len, text, encrypted, rsa, RSA_PKCS1_PADDING);
    return res;
}
int private_decrypt(unsigned char * encText, int len,unsigned char * key, unsigned char *decrypted){

    RSA *rsa = createRSA(key, false);
    int res = RSA_private_decrypt(len, encText, decrypted, rsa, RSA_PKCS1_PADDING);
    return res;
}


int main(){
    unsigned char plainText[2048/8] = "Hello this is Ravi"; //key length : 2048

 unsigned char publicKey[]="-----BEGIN PUBLIC KEY-----\n"\
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy8Dbv8prpJ/0kKhlGeJY\n"\
"ozo2t60EG8L0561g13R29LvMR5hyvGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+\n"\
"vw1HocOAZtWK0z3r26uA8kQYOKX9Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQAp\n"\
"fc9jB9nTzphOgM4JiEYvlV8FLhg9yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68\n"\
"i6T4nNq7NWC+UNVjQHxNQMQMzU6lWCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoV\n"\
"PpY72+eVthKzpMeyHkBn7ciumk5qgLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUy\n"\
"wQIDAQAB\n"\
"-----END PUBLIC KEY-----\n";

 unsigned char privateKey[]="-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEowIBAAKCAQEAy8Dbv8prpJ/0kKhlGeJYozo2t60EG8L0561g13R29LvMR5hy\n"\
"vGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+vw1HocOAZtWK0z3r26uA8kQYOKX9\n"\
"Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQApfc9jB9nTzphOgM4JiEYvlV8FLhg9\n"\
"yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68i6T4nNq7NWC+UNVjQHxNQMQMzU6l\n"\
"WCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoVPpY72+eVthKzpMeyHkBn7ciumk5q\n"\
"gLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUywQIDAQABAoIBADhg1u1Mv1hAAlX8\n"\
"omz1Gn2f4AAW2aos2cM5UDCNw1SYmj+9SRIkaxjRsE/C4o9sw1oxrg1/z6kajV0e\n"\
"N/t008FdlVKHXAIYWF93JMoVvIpMmT8jft6AN/y3NMpivgt2inmmEJZYNioFJKZG\n"\
"X+/vKYvsVISZm2fw8NfnKvAQK55yu+GRWBZGOeS9K+LbYvOwcrjKhHz66m4bedKd\n"\
"gVAix6NE5iwmjNXktSQlJMCjbtdNXg/xo1/G4kG2p/MO1HLcKfe1N5FgBiXj3Qjl\n"\
"vgvjJZkh1as2KTgaPOBqZaP03738VnYg23ISyvfT/teArVGtxrmFP7939EvJFKpF\n"\
"1wTxuDkCgYEA7t0DR37zt+dEJy+5vm7zSmN97VenwQJFWMiulkHGa0yU3lLasxxu\n"\
"m0oUtndIjenIvSx6t3Y+agK2F3EPbb0AZ5wZ1p1IXs4vktgeQwSSBdqcM8LZFDvZ\n"\
"uPboQnJoRdIkd62XnP5ekIEIBAfOp8v2wFpSfE7nNH2u4CpAXNSF9HsCgYEA2l8D\n"\
"JrDE5m9Kkn+J4l+AdGfeBL1igPF3DnuPoV67BpgiaAgI4h25UJzXiDKKoa706S0D\n"\
"4XB74zOLX11MaGPMIdhlG+SgeQfNoC5lE4ZWXNyESJH1SVgRGT9nBC2vtL6bxCVV\n"\
"WBkTeC5D6c/QXcai6yw6OYyNNdp0uznKURe1xvMCgYBVYYcEjWqMuAvyferFGV+5\n"\
"nWqr5gM+yJMFM2bEqupD/HHSLoeiMm2O8KIKvwSeRYzNohKTdZ7FwgZYxr8fGMoG\n"\
"PxQ1VK9DxCvZL4tRpVaU5Rmknud9hg9DQG6xIbgIDR+f79sb8QjYWmcFGc1SyWOA\n"\
"SkjlykZ2yt4xnqi3BfiD9QKBgGqLgRYXmXp1QoVIBRaWUi55nzHg1XbkWZqPXvz1\n"\
"I3uMLv1jLjJlHk3euKqTPmC05HoApKwSHeA0/gOBmg404xyAYJTDcCidTg6hlF96\n"\
"ZBja3xApZuxqM62F6dV4FQqzFX0WWhWp5n301N33r0qR6FumMKJzmVJ1TA8tmzEF\n"\
"yINRAoGBAJqioYs8rK6eXzA8ywYLjqTLu/yQSLBn/4ta36K8DyCoLNlNxSuox+A5\n"\
"w6z2vEfRVQDq4Hm4vBzjdi3QfYLNkTiTqLcvgWZ+eX44ogXtdTDO7c+GeMKWz4XX\n"\
"uJSUVL5+CVjKLjZEJ6Qc2WZLl94xSwL71E41H4YciVnSCQxVc4Jw\n"\
"-----END RSA PRIVATE KEY-----\n";


unsigned char  encrypted[4098]={'\0'};
unsigned char decrypted[4098]={'\0'};

int encrypted_length = public_encrypt(plainText,strlen(plainText),publicKey,encrypted);

printf("Encrypted length =%d\n",encrypted_length);
printf("Encrypted Text =%s\n",encrypted);
int decrypted_length = private_decrypt(encrypted,encrypted_length,privateKey, decrypted);

printf("Decrypted Text =%s\n",decrypted);
printf("Decrypted Length =%d\n",decrypted_length);

}

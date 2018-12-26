#include <sys/socket.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

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
    while ((de = readdir(dr)) != NULL){
            printf("\n\n\n%s : %lu \n", de->d_name, de->d_ino);
            struct stat fileStat;
            stat(de->d_name,&fileStat);
            if(S_ISDIR(fileStat.st_mode)) printf("This file is a deirectry!\n");
            else  printf("This file is not a deirectry!\n");
            if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
                continue;
            if(!S_ISDIR(fileStat.st_mode)){
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
                if(S_ISDIR(fileStat.st_mode)) printf("This file is a deirectry!\n");
                else  printf("This file is not a deirectry!\n");
            }
            else {
                char newPath[200];
                strcpy(newPath, path);
                strcat(newPath, "/");
                strcat(newPath, de->d_name);
                printf("\n\nEntering dir: %s\n\n", newPath);
                readR(newPath);
            }

    }

    closedir(dr);
}

int main(int n, char *args[n]){
     setbuf(stdout, NULL);
       // Pointer for directory entry
     readR("/home/emil/Facultate/Imperative Programming");
    // opendir() returns a pointer of DIR type.
    return 0;
}

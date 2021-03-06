#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "server.h"

void getResp(char *response){

    int sockfd = 0,n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0' ,sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0){
        printf("\n Error : Could not create socket \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = inet_addr("35.246.6.171");

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
        printf("\n Error : Connect Failed!\n");
        return;
    }

    while((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0){
        recvBuff[n] = 0;
        strcat(response, recvBuff);
    }

}

void getKeyId(key_id *ki){
    char response[30000];
    getResp(response);
    char *p = strchr(response, '!');
    int poz = p - response;
    strcpy((char *)ki->key, p+1);
    int id = 0, i = 0;
    while(poz){
        poz--;
        id *= 10;
        id += (response[i] - '0');
        i++;
    }
    ki->id = id;
    //printf("%s\n\n\n%d\n\n%s\n", response, ki->id, ki->key);
}

#ifdef test

void test
    key_id ki;
    ki.id = 0;
    getKeyId(&ki);
    assert(ki.id != 0);
}

#endif

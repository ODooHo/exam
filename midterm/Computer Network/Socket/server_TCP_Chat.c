#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int sSock, listener;
    struct sockaddr_in srvAddr, cliAddr;
    unsigned short srvPort;
    char buffer[BUFFER_SIZE];
    int sentSize, rcvSize;
    unsigned int srvAddrLen, cliAddrLen;
    int ret, i;

    if (argc != 2){
        printf("Usage: %s Port\n",argv[0]);
        exit(0);
    }
    srvPort = atoi(argv[1]);  //서버 포트를 명령 실행줄에서 입력 받음
    // TCP서버 용 소켓 생성, 클라이언트로부터 요청을 받기 위해 사용되는 소켓
    // 클라이언트와 데이터 송수신 시에는 다른 소켓이 사용됨
    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener<0){
        printf("Server socket() failed\n");
        exit(0);
    }
    //서버 주소 설정, 이후 바인드 시스템 콜에서 사용됨
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(srvPort);

    //소켓과 서버 주소를 바인딩
    ret = bind(listener, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if(ret < 0){
        printf("Server cannot bind local addresss.\n");
        exit(0);
    }

    printf("Server is running.\n");
    //클라이언트의 연결을 기다림. 최대 5개의 클라이언트 동시 서비스 가능

    ret = listen(listener,5);
    if(ret < 0){
        printf("Server failed to listen.\n");
        exit(0);
    }

    cliAddrLen = sizeof(cliAddr);
    //클라이언트 연결 수락. 클라이언트와 통신할 새로운 소켓 생성
    sSock = accept(listener,(struct sockaddr *)&cliAddr, &cliAddrLen);
    if(sSock < 0){
        printf("Server failed to accept.\n");
        exit(0);
    }
    printf("Client is connected.\n");

    while(1){
        rcvSize = read(sSock, buffer, BUFFER_SIZE);
        if(rcvSize < 0){
            printf("Error in read()\n");
            exit(0);
        }

        printf("[Client/%s:%d] %s\n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port),buffer);

        if(!strcmp(buffer,"quit")) break;

        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer)-1] = '\0';

        sentSize = write(sSock, buffer, strlen(buffer)+1);
        if(sentSize != strlen(buffer)+1){
            printf("write() sent a different number of bytes than expected\n");
            exit(0);
        }
        if(!strcmp(buffer,"quit")) break;
    }

    close(sSock);
    close(listener);
    printf("Chat Closed.\n");


}
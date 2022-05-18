#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int cSock;
    struct sockaddr_in srvAddr;
    char *srvIp = NULL;
    unsigned short srvPort;
    char buffer[BUFFER_SIZE];
    char temp_1[BUFFER_SIZE];
    char temp_2[BUFFER_SIZE];
    char temp_3[BUFFER_SIZE];
    int sentSize, rcvSize;
    int ret;
    int len1,len2;

    if (argc != 3){
        printf("Usage: %s IP_addr Port\n",argv[0]);
        exit(0);
    }
    srvIp = argv[1]; //서버 IP를 병령 실행줄에서 입력 받음
    srvPort = atoi(argv[2]); //서버 포트를 명령 실행줄에서 입력 받음

    //TCP 클라이언트 용 소켓 생성
    cSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(cSock < 0){
        printf("socket() failed\n");
        exit(0);
    }

    //서버 주소 설정. 이후 서버로 접속 시 connect() 시스템 콜에서 사용
    memset(&srvAddr, 0, sizeof(srvAddr));   //주소를 0으로 초기화
    srvAddr.sin_family = AF_INET;            //IPV4 
    srvAddr.sin_addr.s_addr = inet_addr(srvIp); //서버 IP 지정
    srvAddr.sin_port = htons(srvPort);          //서버 포트 번호 지정

    //서버에 TCP 연결 수행
    ret = connect(cSock,(struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (ret < 0){
        printf("Client cannot connect to the Server.\n");
        exit(0);
    }
    printf("Client is running.\n");
    fgets(temp_1, BUFFER_SIZE, stdin);

    sentSize = send(cSock,buffer, strlen(buffer),0);
        rcvSize = recv(cSock,buffer,BUFFER_SIZE,0);
        if(rcvSize <0){
            printf("Error in read()\n");
            exit(0);
        }

        printf("%s",buffer);

    close(cSock);
}
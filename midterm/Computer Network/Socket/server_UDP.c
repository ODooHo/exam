#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
int sock;  //UDP 서버 쪽 소켓 디스크립터
struct sockaddr_in srvAddr; //서버 주소
struct sockaddr_in cliAddr; //클라이언트 주소 
unsigned short srvPort;
char buffer[BUFFER_SIZE];
int sentSize, rcvSize;
unsigned int srvAddrLen, cliAddrLen;
int ret, i;

if (argc != 2){
    printf("Usage: %s Port\n", argv[0]);
    exit(0);
}
srvPort = atoi(argv[1]); //서버 포트를 입력 받음
// UDP 용 소켓 생성, 이후 소켓 사용시 "sock" 소켓 디스크립터 사용
sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
if(sock < 0) { //예외 처리
    printf("socket() failed\n");
    exit(0);
}

//서버 주소 설정, 이후 바인드 시스템 콜에서 사용됨
memset(&srvAddr, 0, sizeof(srvAddr));
srvAddr.sin_family = AF_INET;
srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
srvAddr.sin_port = htons(srvPort);

//소켓 과 서버 주소 바인딩
ret = bind(sock, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
if(ret < 0){ //예외 처리
    printf("bind() failed\n");
    exit(0);
}

printf("Server is running.\n");

while(1){ //메시지 송수신
    cliAddrLen = sizeof(cliAddr);
    rcvSize = recvfrom(sock,buffer,BUFFER_SIZE, 0, (struct sockaddr *)&cliAddr, &cliAddrLen);
    //소켓 디스크립터,버퍼이름,버퍼 크기, 플래그(보통 0), 수신자 주소, 수신자 주소 구조체 크기
    if (rcvSize < 0){ //예외 처리
        printf("Error in recvfrom()\n");
        exit(0);
    }
    printf("[Client/%s:%d] %s\n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port),buffer);
    if(!strcmp(buffer, "quit")) break;

    for(i=0; buffer[i] != '\0'; i++){
        buffer[i] = toupper(buffer[i]);
    }

    sentSize = sendto(sock, buffer, strlen(buffer)+1,0,(struct sockaddr *)&cliAddr, sizeof(cliAddr));
    
    if(sentSize != strlen(buffer)+1){
        printf("sendto() sent a different number of bytes than expected");
        exit(0);
    }
}
close(sock);
printf("UDP Server is Closed.\n");
}

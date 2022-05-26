#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <winsock.h>
#define BraodcastPort 5678
#define MAXLINE 1024

SOCKET sd;
struct sockaddr_in serv;
char str[MAXLINE];
int channel, serv_len;

void *recvSelectChannel(void);

int main(int argc, char** argv) {

   struct sockaddr_in cli;
   WSADATA 		wsadata;
   int n, threadID;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用

   sd=socket(AF_INET, SOCK_DGRAM, 0);
   bool broadcast = true;
   if(	setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast))<0)
   printf("setsockopt() error!\n");

   cli.sin_family       = AF_INET;
   cli.sin_addr.s_addr  = 0;
   cli.sin_port         = htons(BraodcastPort);
   if( bind(sd, (LPSOCKADDR) &cli, sizeof(cli)) <0 ){
       printf("bind error!\n");
       system("pause");
       return 0;
    }
    serv_len=sizeof(serv);

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) recvSelectChannel, NULL, 0, (LPWORD) &threadID);
    while(1){
        //printf("channel(1~3): ");
        scanf("%d", &channel);
        printf("switch to %d channel\n", channel);
    }
    closesocket(sd); //關閉 socket
    WSACleanup();  // 結束 WinSock DLL 的使用
    return 0;
}

void *recvSelectChannel(void){
    while(1){
        recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR) &serv, &serv_len);
        switch(channel)
        {
            case 1:
                if(str[0] >= '0' && str[0] <= '9')
                    printf("client(from: %s): %s\n", inet_ntoa(serv.sin_addr), str);
                break;
            case 2:
                if(str[0] >= 'a' && str[0] <= 'z')
                    printf("client(from: %s): %s\n", inet_ntoa(serv.sin_addr), str);
                break;
            case 3:
                if(str[0]>='A' && str[0]<='Z')
                    printf("client(from: %s): %s\n", inet_ntoa(serv.sin_addr), str);
                break;
        }
    }
}

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <winsock.h>
#define BroadcastPort 5678
#define MAXLINE 1024

SOCKET sd;
struct sockaddr_in serv;
char str[MAXLINE];
int channel, serv_len, n;

void *recvSelectChannel(void);

int main(int argc, char** argv) {

   struct sockaddr_in cli;
   WSADATA 		wsadata;
   int threadID;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用

   sd=socket(AF_INET, SOCK_DGRAM, 0);
   bool broadcast = true;
   if(	setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast))<0)
   printf("setsockopt() error!\n");

   cli.sin_family       = AF_INET;
   cli.sin_addr.s_addr  = 0;
   cli.sin_port         = htons(BroadcastPort);
   if( bind(sd, (LPSOCKADDR) &cli, sizeof(cli)) <0 ){
       printf("bind error!\n");
       system("pause");
       return 0;
    }
    serv_len=sizeof(serv);

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) recvSelectChannel, NULL, 0, (LPWORD) &threadID);
    while(1){
        scanf("%d", &channel);
        printf("switch to %d channel\n", channel);
        closesocket(sd);
        sd =socket(AF_INET, SOCK_DGRAM, 0);
        if(setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (const char *)&broadcast, sizeof(broadcast)) < 0)
            printf("setsockopt() error!\n");
        cli.sin_family = AF_INET;
        cli.sin_addr.s_addr = 0;
        switch(channel)
        {
            case 1:
                cli.sin_port = htons(BroadcastPort);
                if(bind(sd, (LPSOCKADDR) &cli, sizeof(cli)) < 0){
                    printf("bind error!\n");
                    return 0;
                }
                break;
            case 2:
                cli.sin_port = htons(BroadcastPort+1);
                if(bind(sd, (LPSOCKADDR) &cli, sizeof(cli)) < 0){
                    printf("bind error!\n");
                    return 0;
                }
                break;
            case 3:
                cli.sin_port = htons(BroadcastPort + 2);
                if(bind(sd, (LPSOCKADDR) &cli, sizeof(cli)) < 0){
                    printf("bind error!\n");
                    return 0;
                }
                break;
        }
    }
    closesocket(sd); //關閉 socket
    WSACleanup();  // 結束 WinSock DLL 的使用
    return 0;
}

void *recvSelectChannel(void){
    while(1){
        n = recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR) &serv, &serv_len);
        if(n > 0)
            printf("client(from %s(%d)): %s, %d bytes\n", inet_ntoa(serv.sin_addr), serv.sin_port, str, n);
    }
}

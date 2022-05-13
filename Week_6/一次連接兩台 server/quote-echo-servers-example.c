#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>

#define MAXLINE 1024    /* 字串緩衝區長度 */

int main()
{
    SOCKET        sd, sd2;     /* socket 描述子 */
    struct sockaddr_in    serv, serv2;
    char  str[MAXLINE];
    char str2[] = "abc";
    int   n;
    int port;
    WSADATA wsadata;


   // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
    if (WSAStartup(0x101,(LPWSADATA) &wsadata) != 0) {
        fprintf(stderr,"echo_srv: WSAStartup() fails!");
        exit(1);
    }
    strcpy(str, "127.0.0.1");
    port = 7;

   // 填寫 sockaddr_in 結構 (serv) 。
   // 內容包括：server 的 IP 位址，port number 等等。
    serv2.sin_family       = AF_INET;
    serv2.sin_addr.s_addr  = inet_addr(str);
    serv2.sin_port         = htons(port);

    if ( (sd2=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
        fprintf(stderr,"echo_cli: can't open tcp socket\n");
        exit(1);
    }
    // 連接至 echo server
    if ( connect(sd2, (LPSOCKADDR) &serv2, sizeof(serv2)) == SOCKET_ERROR) {
        fprintf(stderr, "echo_cli: can't connect to echo server\n");
        exit(1);
    }

    for(int i=0; i<10; i++){
        Sleep(100);
        if(send(sd2, str2, sizeof(str2)+1, 0) == SOCKET_ERROR){
            fprintf(stderr, "echo_cli: send() error!\n");
            break;
        }
        if ( (n=recv(sd2, str, MAXLINE, 0))==0) {
          fprintf(stderr,"echo_cli: connection closed\n");
          break;
        } else if (n==SOCKET_ERROR) {
            fprintf(stderr,"echo_cli: recv() error!\n");
            break;
        }
        else printf(str);

        printf("\n==============================\n");
        Sleep(100);
        strcpy(str,"127.0.0.1");
        port = 17;

        /*
        * 填寫 sockaddr_in 結構 (serv) 。
        * 內容包括：server 的 IP 位址，port number 等等。
        */

        serv.sin_family       = AF_INET;
        serv.sin_addr.s_addr  = inet_addr(str);
        serv.sin_port         = htons(port);


        if ( (sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
            fprintf(stderr,"echo_cli: can't open tcp socket\n");
            exit(1);
        }

        /*
        * 連接至 echo server
        */

        if ( connect(sd, (LPSOCKADDR) &serv, sizeof(serv)) == SOCKET_ERROR) {
            fprintf(stderr, "echo_cli: can't connect to echo server\n");
            exit(1);
        }

        n=recv(sd, str, MAXLINE, 0);
        while( n!= 0 ) {
            if (n==SOCKET_ERROR) {
              fprintf(stderr,"echo_cli: recv() error!\n");
              break;
            }
            else
                str[n]='\0';
            printf(str);
            n=recv(sd, str, MAXLINE, 0);
        }

        printf("\n==============================\n");
        //printf("\n\necho_cli: connection closed\n");

        /*
        * 結束 WinSock DLL 的使用
        */

        closesocket(sd);
    }
    closesocket(sd2);
   WSACleanup();
   return 0;
}


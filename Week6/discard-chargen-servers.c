#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>

#define MAXLINE 1024    /* 字串緩衝區長度 */
/*
   sd : discard socket
   sd2 : chargen socket
   serv : discard server struct
   serv2 : chargen server struct
為一個client程式，分別連至Simple TCP/IP Service的Discard與CharGen Server。
該Client會將所有接收來自CharGen Server的封包，都送往Discard Server。
持續印出每秒送往Discard Server的Byte數量
*/
int main()
{
    SOCKET        sd, sd2;     /* socket 描述子 */
    struct sockaddr_in    serv, serv2;
    char  str[MAXLINE];
    int   n;
    int port;
    WSADATA wsadata;

    // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
    if (WSAStartup(0x101,(LPWSADATA) &wsadata) != 0) {
        fprintf(stderr,"echo_srv: WSAStartup() fails!");
        exit(1);
    }
    strcpy(str, "127.0.0.1");
    port = 9;

    // 填寫 sockaddr_in 結構 (serv) 。
   // 內容包括：server 的 IP 位址，port number 等等。
    serv.sin_family       = AF_INET;
    serv.sin_addr.s_addr  = inet_addr(str);
    serv.sin_port         = htons(port);

    if ( (sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
        fprintf(stderr,"discard_cli: can't open tcp socket\n");
        exit(1);
    }
    // 連接至 discard server
    if ( connect(sd, (LPSOCKADDR) &serv, sizeof(serv)) == SOCKET_ERROR) {
        fprintf(stderr, "discard_cli: can't connect to discard server\n");
        exit(1);
    }
    for(int i=0; i<10; i++){
        Sleep(100);
        strcpy(str,"127.0.0.1");
        port = 19;
        /*
        * 填寫 sockaddr_in 結構 (serv) 。
        * 內容包括：server 的 IP 位址，port number 等等。
        */
        serv2.sin_family       = AF_INET;
        serv2.sin_addr.s_addr  = inet_addr(str);
        serv2.sin_port         = htons(port);

        if ( (sd2=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
            fprintf(stderr,"chargen_cli: can't open tcp socket\n");
            exit(1);
        }
        /*
        * 連接至 chargen server
        */
        if ( connect(sd2, (LPSOCKADDR) &serv2, sizeof(serv2)) == SOCKET_ERROR) {
            fprintf(stderr, "chargen_cli: can't connect to chargen server\n");
            exit(1);
        }
        n=recv(sd2, str, MAXLINE, 0);
        while( n!= 0 ) {
            if (n==SOCKET_ERROR) {
              fprintf(stderr,"chargen_cli: recv() error!\n");
              break;
            }
            else{
                str[n]='\0';
                printf("chargen client recv: \n%s", str);
                break;
            }
            n=recv(sd2, str, MAXLINE, 0);
        }
        printf("\n==============================\n");
        Sleep(100);
        if(send(sd, str, sizeof(str)+1, 0) == SOCKET_ERROR){
            fprintf(stderr, "discard_cli: send() error!\n");
            break;
        }
        printf("送往 discard server 長度: %d bytes\n", n);
        printf("\n==============================\n");

        // 關掉 chargen server socket
        closesocket(sd2);
    }
   closesocket(sd);
   WSACleanup();
   return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
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
    int total=0;
    time_t start, in_loop;
    WSADATA wsadata;

    // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
    if (WSAStartup(0x101,(LPWSADATA) &wsadata) != 0) {
        fprintf(stderr,"echo_srv: WSAStartup() fails!");
        exit(1);
    }
    // set up discard server
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(9); // discard-server port
    if( (sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR){
        fprintf(stderr,"discard_cli: can't open tcp socket\n");
        exit(1);
    }
    if(connect(sd, (struct sockaddr *) &serv, sizeof(serv)) == SOCKET_ERROR){
        fprintf(stderr, "discard_cli: can't connect to echo server\n");
        exit(1);
    }

    // set up chargen server
    serv2.sin_family = AF_INET;
    serv2.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv2.sin_port = htons(19); // chargen-server port
    if( (sd2 = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR){
        fprintf(stderr,"chargen_cli: can't open tcp socket\n");
        exit(1);
    }
    if( connect(sd2, (struct sockaddr *) &serv2, sizeof(serv2)) == SOCKET_ERROR){
        fprintf(stderr, "chargen_cli: can't connect to echo server\n");
        exit(1);
    }
    start = time(NULL); // set the start timer
    while(1){
        if( (n = recv(sd2, str, MAXLINE, 0)) ==  SOCKET_ERROR){
            fprintf(stderr,"chargen_cli: recv() error!\n");
            break;
        }
        else if(n == 0){
            fprintf(stderr, "chargen_cli: connection closed\n");
            break;
        }
        n = send(sd, str, strlen(str)+1, 0);
        if(n == SOCKET_ERROR){
            fprintf(stderr, "discard_cli: send() error\n");
            break;
        }
        total += n;
        in_loop = time(NULL); // get the current time everytime run the loop
        if(difftime(in_loop, start) > 1.0){
            printf("send to discard server: %d (bytes)\n", total);
            total = 0;
            start = time(NULL);
        }
    }
    closesocket(sd);
    WSACleanup();
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include <iostream>
#define MAXLINE 1024
using namespace std;

int main(int argc, char** argv) {

   SOCKET        	sd;
   struct sockaddr_in serv;
   char  		str[1024]="abc";
   char  		str_r[1024];
   WSADATA 		wsadata;
   int n;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用

   sd=socket(AF_INET, SOCK_STREAM, 0); //開啟一個 TCP socket.

   //為連線作準備，包含填寫 sockaddr_in 結構 (serv) 。
   //內容有：server 的 IP 位址，port number 等等。
   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(1234);

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // 連接至 echo server

    while(1){
        scanf("%s", str);
        send(sd, str, strlen(str)+1, 0); //傳送how are you至echo server

        n=recv(sd, str_r, MAXLINE, 0); //由echo server接收
        cout << string(str_r) << endl;
        if(strcmp(str, "=") == 0)   break;
    }

   closesocket(sd); //關閉TCP socket

   WSACleanup();  // 結束 WinSock DLL 的使用
   system("pause");

   return 0;
}



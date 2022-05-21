#include <stdio.h>
#include <winsock.h>
#define MAXLINE 1024

int main(int argc, char** argv) {

   SOCKET        	sd;
   struct sockaddr_in serv;
   char  		str[1024]="I love NP!!";
   WSADATA 		wsadata;
   int n;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用

   sd=socket(AF_INET, SOCK_STREAM, 0);
   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(5678);

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // 連接至 echo server

    while(1)
    {
       strcpy( str, "How are you?");
       n = send(sd, str, strlen(str)+1, 0); //傳送###至server
       printf("send:%s\n",str);
       if (n < 0) break;

       Sleep(1000);

       n=recv(sd, str, MAXLINE, 0); //由echo server接收
       if (n <= 0) 	break;
       printf("recv:%s\n",str);
    }
       closesocket(sd); //關閉TCP socket
       WSACleanup();  // 結束 WinSock DLL 的使用
       return 0;
}

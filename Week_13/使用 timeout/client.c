#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {

   SOCKET        	sd;
   struct sockaddr_in serv;
   char  		str[1024]="How are you?";
   char         str_r[1024];
   WSADATA 		wsadata;
   int n,i;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用

   sd=socket(AF_INET, SOCK_STREAM, 0); //開啟一個 TCP socket.

   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(5678);

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // 連接至 echo server
   printf("Client has connectted to Server.\n");

	for (;;){  //每隔10秒，週期送出
	    Sleep(10000);
	   	send(sd, str, strlen(str)+1, 0);
	    printf("Send: %s (in 10 secs)\n" ,str);
	    n = recv(sd, str, MAXLINE, 0);
	    str[n] = '\0';
	    printf("Recv: %s\n", str);
	}
   closesocket(sd); //關閉TCP socket
   WSACleanup();  // 結束 WinSock DLL 的使用
   return 0;
}


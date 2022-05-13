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

   WSAStartup(0x101,(LPWSADATA) &wsadata); // �I�s WSAStartup() ���U WinSock DLL ���ϥ�

   sd=socket(AF_INET, SOCK_STREAM, 0); //�}�Ҥ@�� TCP socket.

   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(5678);

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // �s���� echo server
   printf("Client has connectted to Server.\n");

	for (;;){  //�C�j10��A�g���e�X
	    Sleep(10000);
	   	send(sd, str, strlen(str)+1, 0);
	    printf("Send: %s (in 10 secs)\n" ,str);
	    n = recv(sd, str, MAXLINE, 0);
	    str[n] = '\0';
	    printf("Recv: %s\n", str);
	}
   closesocket(sd); //����TCP socket
   WSACleanup();  // ���� WinSock DLL ���ϥ�
   return 0;
}


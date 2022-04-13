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

   WSAStartup(0x101,(LPWSADATA) &wsadata); // �I�s WSAStartup() ���U WinSock DLL ���ϥ�

   sd=socket(AF_INET, SOCK_STREAM, 0); //�}�Ҥ@�� TCP socket.

   //���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
   //���e���Gserver �� IP ��}�Aport number �����C
   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(1234);

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // �s���� echo server

    while(1){
        scanf("%s", str);
        send(sd, str, strlen(str)+1, 0); //�ǰehow are you��echo server

        n=recv(sd, str_r, MAXLINE, 0); //��echo server����
        cout << string(str_r) << endl;
        if(strcmp(str, "=") == 0)   break;
    }

   closesocket(sd); //����TCP socket

   WSACleanup();  // ���� WinSock DLL ���ϥ�
   system("pause");

   return 0;
}



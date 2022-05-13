#include <stdio.h>
#include <winsock.h>
#define MAXSIZE 1024

int main()
{
	SOCKET sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n;
	char str[1024];

	serv.sin_family= AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(1234);

	WSAStartup(0x101, (LPWSADATA) &wsadata);
	sd = socket(AF_INET, SOCK_DGRAM,0);
	int len_serv=sizeof(serv);

	n = bind(sd,(struct sockaddr*) &serv,len_serv);

	int len_clnt = sizeof(clnt);
	printf("Server waits.\n");
	Sleep(1000);

	while(1){
        n = recv(sd, str, 100, 0);
        printf("recv:%s (%d)\n",str,n);
        Sleep(1000);
	}
	closesocket(sd);
	WSACleanup();
	system("pause");
}

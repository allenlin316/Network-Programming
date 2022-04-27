#include <stdio.h>
#include <winsock.h>
#define MAXSIZE 1024

int main()
{
	SOCKET sd,clnt_sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n;
	char str[1024];

	serv.sin_family= AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(1234);

	WSAStartup(0x101, (LPWSADATA) &wsadata);
	sd = socket(AF_INET, SOCK_STREAM,0);
	int len_serv=sizeof(serv);

	n = bind(sd,(struct sockaddr*) &serv,len_serv);
	n = listen(sd,5);
	int len_clnt = sizeof(clnt);
	clnt_sd = accept(sd,(struct sockaddr*) &clnt,&len_clnt);
	Sleep(1000);
    n = recv(clnt_sd, str, 100, 0);
    printf("server recv:%s (%d)\n",str,n);
	closesocket(sd);
    closesocket(clnt_sd);
	WSACleanup();
	system("pause");
}

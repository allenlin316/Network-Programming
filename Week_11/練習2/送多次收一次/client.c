#include <stdio.h>
#include <winsock.h>

int main()
{
	SOCKET sd;
	WSADATA wsadata;
	struct sockaddr_in serv;
	int i,n;
	char str[1024]="[NCYU]";

	serv.sin_family= AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(1234);

	WSAStartup(0x101, (LPWSADATA) &wsadata);
	sd = socket(AF_INET, SOCK_STREAM,0);
	int len_serv=sizeof(serv);
	n = connect(sd,(struct sockaddr*) &serv, len_serv);
	for(int i=0; i<5; i++){
        n = send(sd, str, strlen(str),0);
        printf("send:%s (%d)\n",str,n);
	}
	closesocket(sd);
	WSACleanup();
	system("pause");
}

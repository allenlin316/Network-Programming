#include <stdio.h>
#include <winsock.h>
#define MAXSIZE 1024

int main()
{
	SOCKET sd;
	WSADATA wsadata;
	struct sockaddr_in serv;
	int i,n;
	char str[1024];
	FILE *fin;
	fin = fopen("in.txt", "rb");

	serv.sin_family= AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(1234);

	WSAStartup(0x101, (LPWSADATA) &wsadata);
	sd = socket(AF_INET, SOCK_DGRAM,0);
	printf("sd=%d\n",sd);
	int len_serv=sizeof(serv);
	n = connect(sd,(struct sockaddr*) &serv, len_serv);
	printf("connect():%d\n",n);

    n = fread(str, 1, MAXSIZE, fin);
	while(n > 0){
        send(sd, str, strlen(str),0);
        printf("read and send: %d bytes\n", n);
        n = fread(str, 1, MAXSIZE, fin);
    }

	closesocket(sd);
	WSACleanup();
	system("pause");
}

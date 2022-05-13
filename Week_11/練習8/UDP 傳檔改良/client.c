#include <stdio.h>
#include <winsock.h>
#define MAXSIZE 2000

int main()
{
	SOCKET sd;
	WSADATA wsadata;
	struct sockaddr_in serv;
	int i,n, sum=0, counter=0;
	char str[MAXSIZE];
	FILE *fin;
	fin = fopen("in.JPG", "rb");

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
        send(sd, str, n,0);
        sum+=n;
        counter++;
        printf("(%d) read and send: %d bytes\n", counter, n);
        n = fread(str, 1, MAXSIZE, fin);
    }
    send(sd, "END", 4, 0);
    printf("File transmission complete. (sum= %d)\n", sum);
    itoa(sum, str, 10);
    send(sd, str, strlen(str), 0);

	closesocket(sd);
	WSACleanup();
	system("pause");
}

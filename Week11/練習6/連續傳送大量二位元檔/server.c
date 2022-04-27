#include <stdio.h>
#include <winsock.h>
#define MAXSIZE 1024

int main()
{
	SOCKET sd,clnt_sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	FILE *fout;
	fout = fopen("out.txt", "w+b");
	int i,n;
	char str[1024];

	serv.sin_family= AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(1234);

	WSAStartup(0x101, (LPWSADATA) &wsadata);
	sd = socket(AF_INET, SOCK_STREAM,0);
	printf("sd=%d\n",sd);
	int len_serv=sizeof(serv);

	n = bind(sd,(struct sockaddr*) &serv,len_serv);
	printf("bind(): %d\n",n);
	n = listen(sd,5);
	printf("listen(): %d\n",n);
	int len_clnt = sizeof(clnt);
	clnt_sd = accept(sd,(struct sockaddr*) &clnt,&len_clnt);


	while( (n = recv(clnt_sd, str, MAXSIZE, 0)) != 0){
        if(n==0)    break;
        str[n] = '\0';
        printf("recv: %d\n", n);
        fwrite(str, 1, strlen(str), fout);
	}

    fclose(fout);
	closesocket(sd);
    closesocket(clnt_sd);
	WSACleanup();
	system("pause");
}

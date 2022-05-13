#include <stdio.h>
#include <winsock.h>
#define MAXSIZE 2000

int main()
{
	SOCKET sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	FILE *fout;
	fout = fopen("out.JPG", "w+b");
	int i,n, sum=0, counter=0;
	char str[MAXSIZE];


	serv.sin_family= AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(1234);

	WSAStartup(0x101, (LPWSADATA) &wsadata);
	sd = socket(AF_INET, SOCK_DGRAM,0);
	printf("sd=%d\n",sd);
	int len_serv=sizeof(serv);

	n = bind(sd,(struct sockaddr*) &serv,len_serv);
	printf("bind(): %d\n",n);
	//n = listen(sd,5);
	//printf("listen(): %d\n",n);
	//int len_clnt = sizeof(clnt);
	//clnt_sd = accept(sd,(struct sockaddr*) &clnt,&len_clnt);

    n = recv(sd, str, MAXSIZE, 0);
	while(n > 0){
        counter++;
        sum+=n;
        fwrite(str, 1, n, fout);
        printf("(%d) recv and save: %d bytes\n", counter, n);
        n = recv(sd, str, MAXSIZE, 0);
        if(strcmp("END", str) == 0){
            printf("File transmission complete. (sum= %d)\n", sum);
            n = recv(sd, str, MAXSIZE, 0);
            if(sum == atoi(str))
                printf("Pkt number matched\n");
            else
                printf("Oops! Pkt number mismatched\n");
            break;
        }
	}

    fclose(fout);
	closesocket(sd);
	WSACleanup();
	system("pause");
}

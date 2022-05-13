// simple TCP quote server

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <winsock.h>

int main(){
	SOCKET sd,clnt_sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n;
	int r;
	char quotes[5][100] = {"\"�V�ܤw�g��ӡA�K�Ѥ��A�����A���M�H�N�S�s�A�ߤ���~�ŷx\"\n\r--�H�W", "\"�@���������A���}�X�Ӫ��A��o�ⱺ�L��ӥV��\"\n\r--S.H.E<�ڷR�A>", "\"�n���̾�����ͬ��M�̻������ڷQ�A�Y�ϩ��ѤѴH�a��\"\n\r--���l"};
	char str[100];

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sd, (struct sockaddr *) &serv, sizeof(serv));

	listen(sd,5);
	while(1){
	    srand(time(NULL));
        r = rand()%3; // random choose number from 0 to 2
        int clnt_len=sizeof(clnt);
        printf("Quote Server waits.\n");
        clnt_sd =  accept(sd, (struct sockaddr *)  &clnt,&clnt_len ); //  wait for client
        printf ("Server sends Quote : %s\n", quotes[r]);
        send(clnt_sd, quotes[r], strlen(quotes[r])+1,0);
        closesocket(clnt_sd);
	}
	closesocket(sd);

	WSACleanup();
}

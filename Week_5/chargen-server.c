// simple TCP chargen server

#include <stdio.h>
#include <winsock.h>

int main(){
	SOCKET sd,clnt_sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n;
	int r;
	int times = 100;
	char chargen[100] = "!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefg";
	char str[100];

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sd, (struct sockaddr *) &serv, sizeof(serv));

	listen(sd,5);
	int clnt_len=sizeof(clnt);
    printf("Chargen Server waits.\n");
    clnt_sd =  accept(sd, (struct sockaddr *)  &clnt,&clnt_len ); //  wait for client
	while(times--){
        printf ("Server sends chars : %s\n", chargen);
        send(clnt_sd, chargen, strlen(chargen)+1,0);
	}
	closesocket(clnt_sd);
	closesocket(sd);

	WSACleanup();
}


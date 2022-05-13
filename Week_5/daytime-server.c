// simple TCP daytime server

#include <stdio.h>
#include <time.h>
#include <winsock.h>

int main(){
    time_t rawtime; // to get the current time
    struct tm * timeinfo;   // to get the current time
	SOCKET sd,clnt_sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n;
	char str[100];
	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sd, (struct sockaddr *) &serv, sizeof(serv));

	listen(sd,5);
	while(1){
        int clnt_len=sizeof(clnt);
        printf("Daytime Server waits.\n");
        clnt_sd =  accept(sd, (struct sockaddr *)  &clnt,&clnt_len );
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        printf ("Server sends time : %s\n", asctime(timeinfo));  // asctime -> Convert tm structure to string
        send(clnt_sd, asctime(timeinfo), strlen(asctime(timeinfo))+1,0);
        closesocket(clnt_sd);
	}
	closesocket(sd);

	WSACleanup();
}




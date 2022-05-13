#include <stdio.h>
#include <winsock.h>
#include <time.h>

int main(){
	SOCKET sd,clnt_sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n;
	char str[100];
	time_t startTime, endTime;
	double duration=0;

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sd, (struct sockaddr *) &serv, sizeof(serv));

	listen(sd,5);
	int clnt_len=sizeof(clnt);
	printf("Server waits.\n");
	clnt_sd =  accept(sd, (struct sockaddr *)  &clnt,&clnt_len );
	printf("Client is connected.\n");
	u_long iMode=1;
    ioctlsocket(clnt_sd,FIONBIO,&iMode);
    startTime = time(NULL);
	while(1){
		n = recv(clnt_sd,str, 100, 0);
		if(n > 0){
            str[n] = '\0';
            printf("(client): %s\n",str);
		}
        endTime = time(NULL);
        if(difftime(endTime, startTime) > 5.0){
            strcpy(str,"I know.");
            printf("(server): %s\n",str);
            send(clnt_sd,str, strlen(str)+1,0);
            startTime = time(NULL);
        }
        Sleep(1000);
	}
	closesocket(sd);
	closesocket(clnt_sd);
	WSACleanup();

}

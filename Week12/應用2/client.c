// simple TCP chat client(1)
// v2: two client chat through server

#include <stdio.h>
#include <winsock.h>
#include <time.h>

int main(){
	SOCKET sd;
	WSADATA wsadata;
	struct sockaddr_in serv;
	int i,n;
	char str[100]="I love NP!";
	char client_name[100];
	int time_interval;
	time_t startTime, endTime;
	double duration;

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sd, (struct sockaddr *)  &serv,sizeof(serv) );
	printf("Connect to server.\n");

    printf("Client name: ");
    gets(client_name);
    printf("Time Interval (sec): ");
    scanf("%d", &time_interval);
	u_long iMode=1;
    ioctlsocket(sd,FIONBIO,&iMode); // enable non-blocking mode
    Sleep(5000);
    startTime = time(NULL);
	while(1){
        endTime = time(NULL);
        if(difftime(endTime, startTime) > time_interval){
            strcpy(str, client_name);
            printf("[Client]: %s\n", str);
            send(sd,str, strlen(str)+1,0);
            startTime = time(NULL);
        }
		memset(str,0,100);
		n = recv(sd,str, 100, 0);
        if(n > 0){
            str[n] = '\0';
            printf("[Others]: %s\n", str);
        }
        Sleep(100);
	}
	closesocket(sd);
	WSACleanup();

}

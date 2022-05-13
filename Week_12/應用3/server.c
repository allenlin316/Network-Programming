#include <stdio.h>
#include <winsock.h>
# define MaxClientNum 5

int main(){
	SOCKET sd,clnt_sd[MaxClientNum];
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n, currentClientNum=0;
	char str[100]="I love NP!";

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sd, (struct sockaddr *) &serv, sizeof(serv));
    listen(sd,5);

    u_long iMode=1;
    ioctlsocket(sd,FIONBIO,&iMode);

	int clnt_len=sizeof(clnt);
	printf("Server waits.\n");

    for(int i=0; i<MaxClientNum; i++)
        clnt_sd[i] = -1;

	while(1){
		n =  accept(sd, (struct sockaddr *)  &clnt,&clnt_len );
		if(n != -1){
            clnt_sd[currentClientNum] = n;
            printf("Client[%d] is connected.\n", currentClientNum);
            currentClientNum++;
		}

		for(int i=0; i<currentClientNum && i<MaxClientNum; i++){
            printf("send to client[%d] : %s\n", i, str);
            send(clnt_sd[i], str, strlen(str)+1, 0);
		}
		Sleep(1000);
	}
	closesocket(sd);
	for(int i=0; i<MaxClientNum; i++)
        if(clnt_sd[i] != -1)
            closesocket(clnt_sd[i]);
	closesocket(clnt_sd);
	WSACleanup();
	system("pause");
}

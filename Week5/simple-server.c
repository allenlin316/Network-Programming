// simple TCP echo server

#include <stdio.h>
#include <winsock.h>

int main(){
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
        printf("Server waits.\n");
        clnt_sd =  accept(sd, (struct sockaddr *)  &clnt,&clnt_len );
        while(1){
            n = recv(clnt_sd,str, 100, 0);
            if(n>0 && str[0]=='.')
                break;
            str[n] = '\0'; // 加上結束字符，才能順利印出字串(才不會有其他不該出現的字元)
            printf("Server recveive:%s\n",str);
            send(clnt_sd,str, strlen(str)+1,0);
            if(n == 0)
                break;
        }
        closesocket(clnt_sd);
	}


	closesocket(sd);

	WSACleanup();

}


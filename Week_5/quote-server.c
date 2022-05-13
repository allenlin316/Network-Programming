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
	char quotes[5][100] = {"\"冬至已經到來，春天不再遙遠，雖然寒意猶存，心中格外溫暖\"\n\r--佚名", "\"一片片落葉，為開出個花園，手牽手捱過整個冬天\"\n\r--S.H.E<我愛你>", "\"要有最樸素的生活和最遙遠的夢想，即使明天天寒地凍\"\n\r--海子"};
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

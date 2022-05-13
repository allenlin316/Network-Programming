//select() sample code

#include<stdio.h>
#include<winsock.h>
#define MAXRECV 1024
#define MAXCLI 10

int main(int argc , char *argv[])
{
    WSADATA wsadata;
    SOCKET serv_sd , cli_sd[MAXCLI], new_socket;
    struct sockaddr_in serv, cli;
    int ready_to_send[MAXCLI];
    int activity, cli_len, i, n, client_num=0;
    fd_set readfds;  //set of socket descriptors
    fd_set writefds;
    char str[MAXRECV];

    for(i=0; i<MAXCLI; i++){
        cli_sd[i] = 0;
        ready_to_send[i] = 0;
    }
    WSAStartup(0x101, &wsadata); //呼叫 WSAStartup() 註冊 WinSock DLL 的使用

    serv_sd=socket(AF_INET, SOCK_STREAM, 0);// 開啟 TCP socket

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons( 5678 );

    bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));

    listen(serv_sd , 3);

//    cli_len1 = sizeof(cli1);
//    cli_len2 = sizeof(cli2);
//    printf("TCP Server waits for client\n");
//    cli_sd1=accept(serv_sd, (LPSOCKADDR) &cli1, &cli_len1);
//    printf("Client1 is connected: socket fd is %d , ip is : %s , port : %d \n" , cli_sd1 , inet_ntoa(cli1.sin_addr) , ntohs(cli1.sin_port));
//    cli_sd2 = accept(serv_sd, (LPSOCKADDR) &cli2, &cli_len2);
//    printf("Client2 is connected: socket fd is %d, ip is : %s, port: %d \n", cli_sd2, inet_ntoa(cli2.sin_addr), ntohs(cli2.sin_port));

while(TRUE)
{
	printf("[1]clear the socket fd set. \n");
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

    printf("[2]add serv_sd to fd set \n");
    FD_SET(serv_sd, &readfds);

	printf("[2]add cli_sd[] to fd set \n");
	for(i=0; i<MAXCLI; i++){
        if(cli_sd[i] > 0){
            if(ready_to_send[i] == 0)
                FD_SET(cli_sd[i], &readfds);
            else
                FD_SET(cli_sd[i], &writefds);
        }
	}

	//wait for an activity on any of the sockets, timeout is NULL , so wait indefinitely
	printf("[3]call select() and waiting \n");
	activity = select( 0 , &readfds , &writefds , NULL , NULL);

	printf("[4]wake up from select(): %d\n",activity);
	if ( activity == SOCKET_ERROR ){
		printf("select call failed with error code: %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	// check serv_sd -> accept():  add new client into cli_sd[]
	if (FD_ISSET( serv_sd , &readfds)){
        cli_len = sizeof(cli);
        new_socket = accept(serv_sd, (struct sockaddr *) &cli, (int *) &cli_len);
        printf("New connection: socket fd is %d, ip is: %s, port: %d \n", new_socket, inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
        for(i=0; i<MAXCLI; i++){
            if(cli_sd[i] == 0){
                cli_sd[i] = new_socket;
                client_num++;
                printf("The %d client socket is in cli_sd[%d]\n", client_num, i);
                break;
            }
        }
	}

	// check each cli_sd client presend in read socket
	for(i=0; i<MAXCLI; i++){
        if(FD_ISSET(cli_sd[i], &readfds)){
            n = recv(cli_sd[i], str, MAXRECV, 0);
            if(n == SOCKET_ERROR){
                int error_code = WSAGetLastError();
                if(error_code == WSAECONNRESET){
                    printf("Host disconnected unexpectedly\n");
                    closesocket(cli_sd[i]);
                    cli_sd[i] = 0;
                    client_num--;
                }
                else
                    printf("recv failed with error code: %d\n", error_code);
            }
            if(n == 0){
                printf("Host disconnected. \n");
                closesocket(cli_sd[i]);
                cli_sd[i] = 0;
                client_num--;
            }
            if(n > 0){
                printf("recv from cli[%d]: %s \n", i, str);
                ready_to_send[i] = 1;
            }
        }
        if(FD_ISSET(cli_sd[i], &writefds)){
            send(cli_sd[i], str, strlen(str)+1, 0);
            ready_to_send[i] = 0;
        }
	}
}// while

    closesocket(serv_sd);
    WSACleanup();
    system("pause");

    return 0;
}

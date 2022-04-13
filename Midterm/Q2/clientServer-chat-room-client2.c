#include <stdio.h>
#include <winsock.h>

int main()
{
    SOCKET sd;
    struct sockaddr_in serv;
    char str[100];
    WSADATA wsadata;
    int n;

    WSAStartup(0x101, &wsadata);
    sd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(1234);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sd, (struct sockaddr *) &serv, sizeof(serv));
    printf("Connect to server\n");

    while(1){
        printf("[Send]: ");
        gets(str);
        send(sd, str, strlen(str)+1, 0);
        memset(str,0,100);
        recv(sd, str, 100, 0);
        printf("[Recv]: %s\n", str);
        if(strcmp(str, "password correct")==0)
            break;
    }
    while(1){
        memset(str, 0, 100);
        recv(sd, str, 100, 0);
        printf("From client1: %s\n", str);
        printf("[client2]: ");
        gets(str);
        send(sd, str, strlen(str)+1, 0);
    }
    closesocket(sd);
    WSACleanup();
}


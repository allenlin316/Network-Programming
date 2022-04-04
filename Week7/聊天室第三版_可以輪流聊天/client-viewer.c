#include <stdio.h>
#include <winsock.h>

#define MAXLINE 1024

int main()
{
    SOCKET sd;
    struct sockaddr_in serv;
    WSADATA wsadata;
    char str[MAXLINE];
    int n;

    WSAStartup(0x101, &wsadata);
    sd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(1234);

    connect(sd, (struct sockaddr *) &serv, sizeof(serv));
    printf("connect to server\n");

    while(1){
        //memset(str, 0, MAXLINE);
        n = recv(sd, str, MAXLINE, 0);
        if(n < 0)   break;
        printf("recv: %s\n", str);
    }
    closesocket(sd);
    WSACleanup();
    return 0;
}

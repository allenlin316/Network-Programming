#include <stdio.h>
#include <winsock.h>

int main()
{
    SOCKET sd;
    struct sockaddr_in serv;
    int i, n;
    char str[100];
    WSADATA wsadata;

    WSAStartup(0x101, &wsadata);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(1234);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sd, (struct sockaddr *) &serv, sizeof(serv));

    printf("connect to server\n");
    while(1){
        printf("From client: ");
        gets(str);
        send(sd, str, strlen(str)+1, 0);
        memset(str, 0, 100);
        recv(sd, str, 100, 0);
        printf("From server: %s\n", str);
    }
    closesocket(sd);
    WSACleanup();
}

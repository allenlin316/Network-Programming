#include <stdio.h>
#include <winsock.h>

int main()
{
    SOCKET sd, clnt_sd;
    struct sockaddr_in serv, clnt;
    int i, n;
    char str[100];
    WSADATA wsadata;

    WSAStartup(0x101, &wsadata);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(1234);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sd, (struct sockaddr *) &serv, sizeof(serv));

    listen(sd, 5);
    int clnt_len = sizeof(clnt);
    printf("Server waits.\n");
    clnt_sd = accept(sd, (struct sockaddr *) &clnt, &clnt_len);
    printf("client is connected\n");
    while(1){
        recv(clnt_sd, str, 100, 0);
        printf("from client: %s\n", str);
        printf("Server send: ");
        gets(str);
        send(clnt_sd, str, strlen(str)+1, 0);
    }
    closesocket(sd);
    closesocket(clnt_sd);
    WSACleanup();
}

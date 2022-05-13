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
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(1234);

    while(1){
        sd = socket(AF_INET, SOCK_STREAM, 0);
        //scanf("Input: %s", &str);
        printf("Input: ");
        gets(str);
        connect(sd, (struct sockaddr *) &serv, sizeof(serv));
        send(sd, str, strlen(str)+1, 0);
        printf("Send: %s\n", str);
        closesocket(sd);
    }
    WSACleanup();
    return 0;
}

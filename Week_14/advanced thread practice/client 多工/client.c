#include <stdio.h>
#include <winsock.h>
#define MAXLINE 1024

void *ThreadMain(void *threadArgs);

struct ThreadArgs
{
    SOCKET sd;
};

int main()
{
    SOCKET sd;
    struct sockaddr_in serv;
    char str[1024];
    WSADATA wsadata;
    DWORD threadID;
    struct ThreadArgs *threadArgs;
    int n;

    WSAStartup(0x101, (LPWSADATA) &wsadata);

    sd = socket(AF_INET, SOCK_STREAM, 0);

    threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    threadArgs->sd = sd;

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5678);

    connect(sd, (LPSOCKADDR) &serv, sizeof(serv));

    if(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs, 0, (LPWORD) &threadID) == NULL)
        printf("CreateThread() failed");
    while(1){
        gets(str);
        n = send(sd, str, strlen(str)+1, 0);
        if(n < 0)
            printf("send error.\n");
    }
}

void *ThreadMain(void *threadArgs){
    SOCKET sd;
    char str[MAXLINE];
    int n;
    sd = ((struct ThreadArgs *)threadArgs)->sd;
    while(1){
        n = recv(sd, str, MAXLINE, 0);
        if(n <= 0)  break;
        printf("recv:%s\n", str);
    }
}


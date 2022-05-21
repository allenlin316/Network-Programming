#include <stdio.h>
#include <winsock.h>

void *ThreadMain(void *arg);

struct ThreadArgs
{
    int clntSock;
};

int main()
{
    struct ThreadArgs *threadArgs;
    LPWORD threadID;
    int servSocket, clntSocket;
    WSADATA wsadata;
    struct sockaddr_in echoServAddr, echoClntAddr;
    int clntLen;
    if(WSAStartup(0x101, &wsadata) != 0){
        printf("WSAStartup() failed");
        exit(1);
    }
     if ((servSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        printf("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(5678);              /* Local port */

    if(bind(servSocket, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        printf("bind() failed");
    if(listen(servSocket, 3) < 0)
        printf("listen() failed");
    printf("Server is waiting for clients.\n");

    while(1){
        clntLen = sizeof(echoClntAddr);
        if((clntSocket = accept(servSocket, &echoClntAddr, &clntLen)) < 0)
            printf("accept() failed");
        threadArgs = (struct ThreadArgs*)malloc(sizeof(struct ThreadArgs));
        threadArgs->clntSock = clntSocket;
       if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs,
              0, (LPDWORD) &threadID) == NULL)
            printf("thread_create() failed");
        printf("New client with thread ID: %ld and socket:%d \n", threadID,clntSocket);
    }
}

void *ThreadMain(void *threadArgs){
    int clntSock;
    clntSock = ((struct ThreadArgs*)threadArgs)->clntSock;
    char echoBuffer[1000];
    int recvMsgSize;
    while(1){
        if((recvMsgSize=recv(clntSock, echoBuffer, 1000, 0))<0){
            printf("Connection[socket:%d] closed\n", clntSock);
            break;
        }
        send(clntSock, echoBuffer, strlen(echoBuffer)+1, 0);
    }
    closesocket(clntSock);
    return(NULL);
}

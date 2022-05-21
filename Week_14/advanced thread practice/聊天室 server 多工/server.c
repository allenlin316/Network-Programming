#include <stdio.h>
#include <winsock.h>
#define MAXCLIENT 10

void *ThreadMain(void *arg);

struct ThreadArgs
{
    int clntSock1;
    int clntSock2;
};

int main()
{
    struct ThreadArgs *threadArgs;
    LPWORD threadID;
    int servSocket, clntSock1, clntSock2;
    WSADATA wsadata;
    struct sockaddr_in echoServAddr, echoClntAddr;
    int clntLen;
    //threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
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

        if((clntSock1 = accept(servSocket, (struct sockaddr *)&echoClntAddr, &clntLen)) < 0)
            printf("clntSock1 accept() failed");
        if((clntSock2 = accept(servSocket, (struct sockaddr *)&echoClntAddr, &clntLen)) < 0)
            printf("clntSock2 accept() failed");

        threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
        threadArgs->clntSock1 = clntSock1;
        threadArgs->clntSock2 = clntSock2;

       if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs,
              0, (LPDWORD) &threadID) == NULL)
            printf("thread_create() failed");
        printf("New chat room with thread ID : %d and sockets %d and %d\n", threadID, clntSock1, clntSock2);
    }
}

void *ThreadMain(void *threadArgs){
    int clntSock1 = ((struct ThreadArgs *)threadArgs)->clntSock1;
    int clntSock2 = ((struct ThreadArgs *)threadArgs)->clntSock2;
    char echoBuffer[1000];
    int recvMsgSize;

    u_long iMode = 1;
    ioctlsocket(clntSock1, FIONBIO, &iMode);
    ioctlsocket(clntSock2, FIONBIO, &iMode);
    int nError;
    while(1){
        if((recvMsgSize=recv(clntSock1, echoBuffer, 1000, 0))<=0)
            send(clntSock2, echoBuffer, strlen(echoBuffer)+1, 0);
        nError = WSAGetLastError();
        if(nError != WSAEWOULDBLOCK && nError!=0){
            printf("Disconnected! error code : %d\n", nError);
            break;
        }

        if((recvMsgSize=recv(clntSock2, echoBuffer, 1000, 0))<=0)
            send(clntSock1, echoBuffer, strlen(echoBuffer)+1, 0);
        nError = WSAGetLastError();
        if(nError != WSAEWOULDBLOCK && nError!=0){
            printf("Disconnected! error code : %d\n", nError);
            break;
        }
        Sleep(1000);
    }
    closesocket(clntSock1);
    closesocket(clntSock2);
    free(threadArgs);
    return(NULL);
}


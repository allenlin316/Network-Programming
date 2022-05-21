#include <stdio.h>
#include <winsock.h>
#define MAXCLIENT 10

void *ThreadMain(void *arg);

struct ThreadArgs
{
    int clntSock[MAXCLIENT];
    int totalClnt;
    int clntIndex;
};

int main()
{
    struct ThreadArgs *threadArgs;
    LPWORD threadID;
    int servSocket, clntSocket;
    WSADATA wsadata;
    struct sockaddr_in echoServAddr, echoClntAddr;
    int clntLen;
    threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    if(WSAStartup(0x101, &wsadata) != 0){
        printf("WSAStartup() failed");
        exit(1);
    }
    for(int i=0; i<MAXCLIENT; i++)
        threadArgs->clntSock[i] = 0;
    threadArgs->totalClnt=0;
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
        int i;
        clntLen = sizeof(echoClntAddr);
        for(i=0; i<MAXCLIENT; i++){
            if(threadArgs->clntSock[i] == 0)
                break;
        }
        if((clntSocket = accept(servSocket, &echoClntAddr, &clntLen)) < 0)
            printf("accept() failed");
        threadArgs->clntIndex = i;
        threadArgs->clntSock[i] = clntSocket;
        threadArgs->totalClnt++;
       if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs,
              0, (LPDWORD) &threadID) == NULL)
            printf("thread_create() failed");
        printf("New client with thread ID: %ld and socket:%d, Current client number is %d \n", threadID,clntSocket, threadArgs->totalClnt);
    }
}

void *ThreadMain(void *threadArgs){
    int clntIndex = ((struct ThreadArgs *)threadArgs)->clntIndex;
    int clntSock = ((struct ThreadArgs *)threadArgs)->clntSock[clntIndex];
    char echoBuffer[1000];
    int recvMsgSize;
    while(1){
        if((recvMsgSize=recv(clntSock, echoBuffer, 1000, 0))<=0){
            ((struct ThreadArgs *)threadArgs)->clntSock[clntIndex]=0;
            ((struct ThreadArgs *)threadArgs)->totalClnt--;
            printf("Connection[socket:%d] closed. Current client number is %d \n", clntSock, ((struct ThreadArgs *)threadArgs)->totalClnt);
            break;
        }
        send(clntSock, echoBuffer, strlen(echoBuffer)+1, 0);
    }
    closesocket(clntSock);
    return(NULL);
}

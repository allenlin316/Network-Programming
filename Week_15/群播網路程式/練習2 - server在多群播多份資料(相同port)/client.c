#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <winsock.h>
#define MAXLINE 1024

SOCKET sd;
struct sockaddr_in serv;
char str[MAXLINE];
int serv_len, n;

void *recvMsg(void){
    while(1){
        serv_len = sizeof(serv);
        n = recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR) &serv, &serv_len);
        if(n > 0)
            printf("client (from: %s): %s\n", inet_ntoa(serv.sin_addr), str);
    }
}

int main()
{
    struct sockaddr_in cli;
    WSADATA wsadata;
    int n;
    int flag=1, len=sizeof(int), channel;
    pthread_t thread1;

    WSAStartup(0x101, (LPWSADATA) &wsadata);
    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (const char *)&flag, len) < 0)
        printf("setsocket() failed");

    cli.sin_family = AF_INET;
    cli.sin_addr.s_addr = 0;
    cli.sin_port = htons(5678);

    if(bind(sd, (LPSOCKADDR) &cli, sizeof(cli)) < 0)
        printf("bind error! \n");

    pthread_create(&thread1, NULL, &recvMsg, NULL);

    // specify the multicast group
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.1");
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY); // will retrieve this computer's interface card

    // join the multicast address
    if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*) &multicastRequest, sizeof(multicastRequest)) < 0)
        printf("setsockopt() failed");

    while(1){
        scanf("%d", &channel);
        printf("switch to %d channel\n", channel);
        switch(channel)
        {
            case 1:
                multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.1");
                printf("join the group %d\n\n", 1);
                if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*) &multicastRequest, sizeof(multicastRequest)) < 0)
                    printf("setsockopt() failed");
                break;
            case 2:
                multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.1");
                printf("drop the group %d\n\n", 1);
                if(setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char*) &multicastRequest, sizeof(multicastRequest)) < 0)
                    printf("setsockopt() failed");
                break;
            case 3:
                multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.2");
                printf("join the group %d\n\n", 2);
                if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*) &multicastRequest, sizeof(multicastRequest)) < 0)
                    printf("setsockopt() failed");
                break;
            case 4:
                multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.2");
                printf("drop the group %d\n\n", 2);
                if(setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char*) &multicastRequest, sizeof(multicastRequest)) < 0)
                    printf("setsockopt() failed");
                break;
            case 5:
                multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.3");
                printf("join the group %d\n\n", 3);
                if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*) &multicastRequest, sizeof(multicastRequest)) < 0)
                    printf("setsockopt() failed");
                break;
            case 6:
                multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.3");
                printf("drop the group %d\n\n", 3);
                if(setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char*) &multicastRequest, sizeof(multicastRequest)) < 0)
                    printf("setsockopt() failed");
                break;
        }
    }
    closesocket(sd);
    WSACleanup();
    return 0;
}


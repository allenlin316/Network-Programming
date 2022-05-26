#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main()
{
    SOCKET sd;
    struct sockaddr_in serv, cli;
    char str[MAXLINE];
    WSADATA wsadata;
    int n, serv_len;
    int flag=1, len=sizeof(int);

    WSAStartup(0x101, (LPWSADATA) &wsadata);
    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (const char *)&flag, len) < 0)
        printf("setsocket() failed");

    cli.sin_family = AF_INET;
    cli.sin_addr.s_addr = 0;
    cli.sin_port = htons(5678);

    if(bind(sd, (LPSOCKADDR) &cli, sizeof(cli)) < 0)
        printf("bind error! \n");

    // specify the multicast group
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.1");
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY); // will retrieve this computer's interface card

    // join the multicast address
    if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*) &multicastRequest, sizeof(multicastRequest)) < 0)
        printf("setsockopt() failed");
    serv_len = sizeof(serv);
    while(1){
        n = recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR) &serv, &serv_len);
        str[n] = '\0';
        printf("client(from: %s): %s\n", inet_ntoa(serv.sin_addr), str);
    }
    closesocket(sd);
    WSACleanup();
    return 0;
}

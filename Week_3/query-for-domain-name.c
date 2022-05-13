#include <stdio.h>
#include <winsock.h>

// query for domain name via DNS server

int main(void)
{
    char ip_str[40];
    WSADATA wsadata;
    int nResult = WSAStartup (0x101, (LPWSADATA) &wsadata);
    if(nResult != 0){
        printf("WSA Initialization failed: %d\n", nResult);
        WSACleanup();
        return 0;
    }
    LPHOSTENT hp;
    struct in_addr sAddr;
    printf("Input IP addr:");
    scanf("%s", ip_str);
    sAddr.s_addr = inet_addr(ip_str); // turn string to 32-bit IP addr.

    hp = gethostbyaddr((LPSTR) &sAddr, sizeof(sAddr), AF_INET); // return host entity, including name, aliases...

    if(hp==NULL)
        printf("get hp error, code:%d\n", WSAGetLastError());
    else{
        printf("host name: %s\n", hp->h_name);
        printf("host nickname: %s\n", hp->h_aliases[0]);
        printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
    }
}

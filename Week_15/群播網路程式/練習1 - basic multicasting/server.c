#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 20    /* 字串緩衝區長度 */

int main()
{
	SOCKET	serv_sd;        /* socket 描述子 */
  	int   	cli_len, n,i=1;
  	char  	str[MAXLINE];

  	struct 	sockaddr_in   	serv, cli;
  	WSADATA wsadata;

    WSAStartup(0x101, &wsadata); //呼叫 WSAStartup() 註冊 WinSock DLL 的使用

   	serv_sd=socket(AF_INET, SOCK_DGRAM, 0);// 開啟 UDP socket


   	/* Set TTL of multicast packet */
   	int multicastTTL=1;
    if (setsockopt(serv_sd, IPPROTO_IP, IP_MULTICAST_TTL, (char *) &multicastTTL,
          sizeof(multicastTTL)) == SOCKET_ERROR)
    printf("setsockopt() failed");

   	cli_len = sizeof(cli);

   	printf("server will multicastcast.\n");

    cli.sin_family      = AF_INET;
   	cli.sin_addr.s_addr = inet_addr("224.1.1.1");
   	cli.sin_port        = htons(5678);

while(1){
		memset(str, i%10 +'0', sizeof(str));
		sendto(serv_sd, str, strlen(str), 0,(LPSOCKADDR)&cli,cli_len);
        printf("server multicast: %s\n",str);		// 顯示送去client 的字串
        Sleep(1000);
        i++;
    }

	//結束 WinSock DLL 的使用
   	closesocket(serv_sd);
   	//closesocket(cli_sd);
   	WSACleanup();
}

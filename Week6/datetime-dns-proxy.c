#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <string.h>

#define MAXLINE 1024    /* �r��w�İϪ��� */

/*
�o�O�@�Ӥ��~�{���A�ҥ�N�z���A��( proxy server)���ѥH�U��ӪA��
    (1) datetime�A��: ����qclient�ǨӦr��utime�v�ɡA�|�h�߰�Simple TCP/IP datetime server���ɶ��A�A�ǰe��client
    (2) ip�d��dns�A�ȡG����qclient�ǨӦr��uask://ip��}�v�ɡA���~�{���|�I�sgethostbyaddr()�d�߫�A�A�N��ip��domain name�^�е�client�C
        �Ҧpclient�e�X�r��Gask://8.8.8.8 �A���~�{���|�^�СA��client�|����æL�X dns.google
*/

void dateTimeQuery(SOCKET *cli_sd);
void dnsQuery(char *query_ip, SOCKET *cli_sd);

int main(void)
{
      SOCKET        serv_sd, cli_sd,sd;        /* socket �y�z�l */
      struct sockaddr_in    serv, cli,dest;
      int   cli_len, n,port;
      char str[MAXLINE];
      char target[MAXLINE];
      WSADATA wsadata;

    // WSAStartup function initiates use of the Winsock DLL by a process.
  if (WSAStartup(0x101,(LPWSADATA) &wsadata) != 0) {
    fprintf(stderr,"echo_srv: can't use WinSock DLL\n");
    exit(1);
  }
    // creates a socket that is bound to a specific transport service provider
  if ( (serv_sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr,"echo_srv: can't open TCP socket\n");
    exit(1);
  }
   serv.sin_family      = AF_INET;
   serv.sin_addr.s_addr = 0;
   serv.sin_port        = htons(1234);
    // bind : associates a local address with a socket.
   if ( bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv)) <0) {
     fprintf(stderr, "echo_srv: can't bind local address\n");
     exit(1);
   }
    /* �I�s listen() �� socket �i�J�u��ť�v���A�A�ë��w
    *   �̦h�i�P�ɱ������s���n�D�]�b��C���^*/
   if ( listen(serv_sd, 5) < 0) {
     fprintf(stderr,"echo_srv: listen() error\n");
     exit(1);
   }

   cli_len = sizeof(cli);
   while(1){
        printf("Proxy server wait for client\n");
        // permits an incoming connection attempt on a socket. return SOCKET data type
        if((cli_sd = accept(serv_sd, (struct sockaddr *)&cli, &cli_len)) == SOCKET_ERROR ){
            fprintf(stderr, "proxy_srv: accpet() error\n");
            closesocket(cli_sd);
        }
        else{
            while(1){
                // recv function receives data from a connected socket or a bound connectionless socket.
                if ( (n=recv(cli_sd, str, MAXLINE, 0))==0) {
                    fprintf(stderr, "proxy_srv: connection closed\n");
                    break;
                } else if (n==SOCKET_ERROR) {
                    fprintf(stderr, "proxy_srv: recv() error!\n");
                    break;
                }
                printf("From Client: %s\n", str);   // ��ܱq client �ǨӪ��r��
                if(strcmp(str, "time") == 0){
                    dateTimeQuery(cli_sd);
                }
                else if(strlen(str) > 6 && strcmp(strncpy(target, strstr(str, "a"), 6), "ask://") == 0){
                    dnsQuery(strstr(str, "/")+2, cli_sd);
                }
                else{
                    printf("Invalid request\n");
                    break;
                }
                printf("=========================\n");
            }
        }
   }
    closesocket(serv_sd);
    closesocket(cli_sd);
    closesocket(sd);
    WSACleanup();
    return 0;
}

void dateTimeQuery(SOCKET *cli_sd){
    char *str;
    int n;
    struct sockaddr_in    serv;
    SOCKET serv_sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(13);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	// establishes a connection to a specified socket.
    connect(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));

    if ( (n=recv(serv_sd, str, MAXLINE, 0))==0) {
        fprintf(stderr, "dateTime_srv: connection closed\n");
        return;
    } else if (n==SOCKET_ERROR) {
        fprintf(stderr, "dateTime_srv: recv() error!\n");
        return;
    }
    str[n] = '\0';  // �[�W�����r���A�H���X�{���ӥX�{���r
    printf("dateTime server send: %s\n", str);
     // sends data on a connected socket.
    send(cli_sd, str, strlen(str)+1, 0);
}

void dnsQuery(char *query_ip, SOCKET *cli_sd){
    LPHOSTENT hp;
    struct in_addr sAddr;   // in_addr structure represents an IPv4 Internet address.
    sAddr.s_addr = inet_addr(query_ip); // turn string to 32-bit IP addr.

    hp = gethostbyaddr((LPSTR) &sAddr, sizeof(sAddr), AF_INET); // return host entity, including name, aliases...

    if(hp==NULL){
        printf("get hp error, code:%d\n", WSAGetLastError());
        return;
    }
    else{
        printf("Dns Server send host name: %s\n", hp->h_name);
        send(cli_sd, hp->h_name, strlen(hp->h_name)+1, 0);
    }
}


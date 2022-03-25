#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>

#define MAXLINE 1024    /* 字串緩衝區長度 */

int main()
{
  SOCKET        sd;     /* socket 描述子 */
  struct sockaddr_in    serv;
  char  str[MAXLINE];
  int   n;
  int port;
  WSADATA wsadata;

  /*
   * 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
   */

  if (WSAStartup(0x101,(LPWSADATA) &wsadata) != 0) {
    fprintf(stderr,"echo_srv: WSAStartup() fails!");
    exit(1);
  }



  /*
   * 開啟一個 TCP socket.
   */
  for(int i=0; i < 20; i++ ) {
      strcpy(str,"127.0.0.1");
      if (i%2 ==0 )
        port = 17;
      else
        port = 13;
      /*
       * 填寫 sockaddr_in 結構 (serv) 。
       * 內容包括：server 的 IP 位址，port number 等等。
       */
      serv.sin_family       = AF_INET;
      serv.sin_addr.s_addr  = inet_addr(str);
      serv.sin_port         = htons(port);

      if ( (sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
        fprintf(stderr,"echo_cli: can't open tcp socket\n");
        exit(1);
      }

      /*
       * 連接至 echo server
       */

      if ( connect(sd, (LPSOCKADDR) &serv, sizeof(serv))
                                                    == SOCKET_ERROR) {
        fprintf(stderr, "echo_cli: can't connect to echo server\n");
        exit(1);
      }

      n=recv(sd, str, MAXLINE, 0);
      while( n!= 0 ) {
        if (n==SOCKET_ERROR) {
          fprintf(stderr,"echo_cli: recv() error!\n");
          break;
        }
        else
            str[n]='\0';
            printf(str);
        n=recv(sd, str, MAXLINE, 0);
      }

      printf("\n==============================\n");
      //printf("\n\necho_cli: connection closed\n");

       /*
        * 結束 WinSock DLL 的使用
        */

       closesocket(sd);
    } // end for
   WSACleanup();
   return 0;
}

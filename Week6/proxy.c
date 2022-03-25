#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>

#define MAXLINE 1024    /* 字串緩衝區長度 */

int main()
{
  SOCKET        serv_sd, cli_sd,sd;        /* socket 描述子 */
  struct sockaddr_in    serv, cli,dest;
  int   cli_len, n,port;
  char  str[MAXLINE];
  WSADATA wsadata;

  /*
   * 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
   */

  if (WSAStartup(0x101,(LPWSADATA) &wsadata) != 0) {
    fprintf(stderr,"echo_srv: can't use WinSock DLL\n");
    exit(1);
  }
// add here
// connect to dest
  strcpy(str,"127.0.0.1");
  port = 7;

  dest.sin_family       = AF_INET;
  dest.sin_addr.s_addr  = inet_addr(str);
  dest.sin_port         = htons(port);

  /*
   * 開啟一個 TCP socket.
   */

  if ( (sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr,"echo_cli: can't open tcp socket\n");
    exit(1);
  }

  /*
   * 連接至 echo server
   */

  if ( connect(sd, (LPSOCKADDR) &dest, sizeof(dest))
                                                == SOCKET_ERROR) {
    fprintf(stderr, "echo_cli: can't connect to echo server\n");
    exit(1);
  }


  /*
   * 開啟 TCP socket
   */

  if ( (serv_sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr,"echo_srv: can't open TCP socket\n");
    exit(1);
  }

  /*
   * 指定 socket 的 IP 位址和 port number
   */

   serv.sin_family      = AF_INET;
   serv.sin_addr.s_addr = 0;
   //serv.sin_port        = htons(IPPORT_ECHO);
   serv.sin_port        = htons(7777);
   // 指定 IPPORT_ECHO 為 echo port

   if ( bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv)) <0) {
     fprintf(stderr, "echo_srv: can't bind local address\n");
     exit(1);
   }


   /*
    * 呼叫 listen() 使 socket 進入「監聽」狀態，並指定
    * 最多可同時接受的連接要求（在佇列中）
   */

   if ( listen(serv_sd, 5) < 0) {
     fprintf(stderr,"echo_srv: listen() error\n");
     exit(1);
   }


   /*
    * 等待 clinet 程式的連接。
    * 注意！我們只允許一個 client 程式的連接
    */

    cli_len=sizeof(cli);
    while (1) {
      printf("echo_srv: waiting for client\n");
      if ( (cli_sd=accept(serv_sd, (struct sockaddr *)
            &cli, &cli_len)) == SOCKET_ERROR ) {
        fprintf(stderr, "echo_srv: accpet() error\n");
        closesocket(cli_sd);
      } else {
        while (1) {
          if ( (n=recv(cli_sd, str, MAXLINE, 0))==0) {
            fprintf(stderr, "echo_srv: connection closed\n");
            break;
          } else if (n==SOCKET_ERROR) {
            fprintf(stderr, "echo_srv: recv() error!\n");
            break;
          }
          str[n]='\0';
          printf("echo_srv from client: %s\n",str);   // 顯示從 client 傳來的字串
          //add here 2
          //send to dest
          if ( send(sd, str, strlen(str), 0) == SOCKET_ERROR) {
            fprintf(stderr, "echo_srv: connection closed\n");
            break;
          }
          //recv from dest
          if ( (n=recv(sd, str, MAXLINE, 0))==0) {
            fprintf(stderr, "echo_srv: connection closed\n");
            break;
          } else if (n==SOCKET_ERROR) {
            fprintf(stderr, "echo_srv: recv() error!\n");
            break;
          }
          str[n]='\0';
          printf("echo_srv from dest: %s\n",str);   // 顯示從 client 傳來的字串

          //add complete
          if ( send(cli_sd, str, strlen(str), 0) == SOCKET_ERROR) {
            fprintf(stderr, "echo_srv: connection closed\n");
            break;
          }
        }
      }
    }

   /*
    * 結束 WinSock DLL 的使用
    */
   closesocket(serv_sd);
   closesocket(cli_sd);
   closesocket(sd);
   WSACleanup();

   return 0;
}

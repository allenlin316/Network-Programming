#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>

#define MAXLINE 1024    /* �r��w�İϪ��� */
/*
   sd : discard socket
   sd2 : chargen socket
   serv : discard server struct
   serv2 : chargen server struct
���@��client�{���A���O�s��Simple TCP/IP Service��Discard�PCharGen Server�C
��Client�|�N�Ҧ������Ӧ�CharGen Server���ʥ]�A���e��Discard Server�C
����L�X�C��e��Discard Server��Byte�ƶq
*/
int main()
{
    SOCKET        sd, sd2;     /* socket �y�z�l */
    struct sockaddr_in    serv, serv2;
    char  str[MAXLINE];
    int   n;
    int port;
    WSADATA wsadata;

    // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
    if (WSAStartup(0x101,(LPWSADATA) &wsadata) != 0) {
        fprintf(stderr,"echo_srv: WSAStartup() fails!");
        exit(1);
    }
    strcpy(str, "127.0.0.1");
    port = 9;

    // ��g sockaddr_in ���c (serv) �C
   // ���e�]�A�Gserver �� IP ��}�Aport number �����C
    serv.sin_family       = AF_INET;
    serv.sin_addr.s_addr  = inet_addr(str);
    serv.sin_port         = htons(port);

    if ( (sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
        fprintf(stderr,"discard_cli: can't open tcp socket\n");
        exit(1);
    }
    // �s���� discard server
    if ( connect(sd, (LPSOCKADDR) &serv, sizeof(serv)) == SOCKET_ERROR) {
        fprintf(stderr, "discard_cli: can't connect to discard server\n");
        exit(1);
    }
    for(int i=0; i<10; i++){
        Sleep(100);
        strcpy(str,"127.0.0.1");
        port = 19;
        /*
        * ��g sockaddr_in ���c (serv) �C
        * ���e�]�A�Gserver �� IP ��}�Aport number �����C
        */
        serv2.sin_family       = AF_INET;
        serv2.sin_addr.s_addr  = inet_addr(str);
        serv2.sin_port         = htons(port);

        if ( (sd2=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
            fprintf(stderr,"chargen_cli: can't open tcp socket\n");
            exit(1);
        }
        /*
        * �s���� chargen server
        */
        if ( connect(sd2, (LPSOCKADDR) &serv2, sizeof(serv2)) == SOCKET_ERROR) {
            fprintf(stderr, "chargen_cli: can't connect to chargen server\n");
            exit(1);
        }
        n=recv(sd2, str, MAXLINE, 0);
        while( n!= 0 ) {
            if (n==SOCKET_ERROR) {
              fprintf(stderr,"chargen_cli: recv() error!\n");
              break;
            }
            else{
                str[n]='\0';
                printf("chargen client recv: \n%s", str);
                break;
            }
            n=recv(sd2, str, MAXLINE, 0);
        }
        printf("\n==============================\n");
        Sleep(100);
        if(send(sd, str, sizeof(str)+1, 0) == SOCKET_ERROR){
            fprintf(stderr, "discard_cli: send() error!\n");
            break;
        }
        printf("�e�� discard server ����: %d bytes\n", n);
        printf("\n==============================\n");

        // ���� chargen server socket
        closesocket(sd2);
    }
   closesocket(sd);
   WSACleanup();
   return 0;
}

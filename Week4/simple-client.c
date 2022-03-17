#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int getTelnetServiceNum(int input_num);

int main(int argc, char** argv) {

   SOCKET        	sd;
   struct sockaddr_in serv;
   char  		str[1024]="I love NCYU CSIE";
   char  		str_r[1024];
   WSADATA 		wsadata;
   int n, input_num, service_num;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // �I�s WSAStartup() ���U WinSock DLL ���ϥ�

   sd=socket(AF_INET, SOCK_STREAM, 0); //�}�Ҥ@�� TCP socket.

   // 5�تA�� menu
   printf("�п�J�A�Ȫ��N��(1~5)�H����A��\n");
   printf("1. �^���A�� (Echo)\n");
   printf("2. ���A�� (Discard)\n");
   printf("3. ����ɶ��A�� (Daytime)\n");
   printf("4. �C��@�y (QUOTE)\n");
   printf("5. �r�����;� (CHARGEN)\n");


    scanf("%d", &input_num);     // Ū�J�ϥΪ̪��N��
    service_num = getTelnetServiceNum(input_num);   // ���o�s�u�� port num

   //���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
   //���e���Gserver �� IP ��}�Aport number �����C
   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(service_num);  // htons = host to network

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // �ݨϥΪ̿�ܤ���\��A�s���ܸ� server

   if(input_num == 1 || input_num == 2){
        send(sd, str, strlen(str)+1, 0); //�ǰe�� Echo/Discard server
        printf("�ǰe���e: %s\n" ,str);
        printf("�ǰe���e����: %d bytes\n\n", strlen(str)+1);
   }
   else{
        printf("�ǰe���e: \n");
        printf("�ǰe���e����: 1 bytes\n\n");
   }

   if(input_num == 2){
        printf("�^�����e: \n");
        printf("�^�����e����: 1 bytes\n");
   }
   else{
        n=recv(sd, str_r, MAXLINE, 0); //����
        printf("�^�����e: %s \n",str_r);
        printf("����: %d\n", strlen(str_r));
        printf("�^�����e����: %d bytes\n", n);
   }

   closesocket(sd); //����TCP socket

   WSACleanup();  // ���� WinSock DLL ���ϥ�
   system("pause");

   return 0;
}

int getTelnetServiceNum(int input_num){
    switch(input_num)
    {
        case 1:
            return 7;
            break;
        case 2:
            return 9;
            break;
        case 3:
            return 13;
            break;
        case 4:
            return 17;
            break;
        case 5:
            return 19;
            break;
    }
    return -1;
}


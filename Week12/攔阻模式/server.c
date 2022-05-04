#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* �r��w�İϪ��� */

int main()
{
	SOCKET	serv_sd, cli_sd;        /* socket �y�z�l */
  	int   	cli_len, n,i;
  	char  	str[MAXLINE];

  	struct 	sockaddr_in   	serv, cli;
  	WSADATA wsadata;

    WSAStartup(0x101, &wsadata); //�I�s WSAStartup() ���U WinSock DLL ���ϥ�

  	serv_sd=socket(AF_INET, SOCK_STREAM, 0);// �}�� TCP socket

   	//���w socket �� IP ��}�M port number
   	serv.sin_family      = AF_INET;
   	serv.sin_addr.s_addr = 0;
   	serv.sin_port        = htons(5678);	// ���w IPPORT_ECHO �� echo port

    bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));

   	listen(serv_sd, 5) ; //�I�s listen() �� socket �i�J�u��ť�v���A

   	cli_len = sizeof(cli);

   	while (1) {
   		printf("server: waiting for client\n");

   		cli_sd=accept(serv_sd, (LPSOCKADDR) &cli, &cli_len);

                //enable non-blocking mode when iMode=1
                u_long iMode=1;
                ioctlsocket(cli_sd,FIONBIO,&iMode);

		if (iMode ==0)
                    printf("This is blocking mode.\n");
		else
                    printf("This is non-blocking mode.\n");

		for (;;)
		{
	        printf("recv() checks.\n");
			n=recv(cli_sd, str, MAXLINE, 0);
			str[n]='\0';

			if (n > 0 )
	        	printf("Recv: %s\n",str);    	// ��ܱq client �ǨӪ��r��

			Sleep(1000);

			int nError=WSAGetLastError();
			if(nError!=WSAEWOULDBLOCK && nError!=0)
			{
				printf("Disconnected! error code:%d\n",nError);
				closesocket(cli_sd);
				break;
			}
        }

    }

	//���� WinSock DLL ���ϥ�
   	closesocket(serv_sd);
   	closesocket(cli_sd);
   	WSACleanup();
}

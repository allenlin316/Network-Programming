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

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用

   sd=socket(AF_INET, SOCK_STREAM, 0); //開啟一個 TCP socket.

   // 5種服務 menu
   printf("請輸入服務的代號(1~5)以執行服務\n");
   printf("1. 回應服務 (Echo)\n");
   printf("2. 丟棄服務 (Discard)\n");
   printf("3. 日期時間服務 (Daytime)\n");
   printf("4. 每日一句 (QUOTE)\n");
   printf("5. 字元產生器 (CHARGEN)\n");


    scanf("%d", &input_num);     // 讀入使用者的代號
    service_num = getTelnetServiceNum(input_num);   // 取得連線的 port num

   //為連線作準備，包含填寫 sockaddr_in 結構 (serv) 。
   //內容有：server 的 IP 位址，port number 等等。
   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(service_num);  // htons = host to network

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // 看使用者選擇什麼功能，連接至該 server

   if(input_num == 1 || input_num == 2){
        send(sd, str, strlen(str)+1, 0); //傳送至 Echo/Discard server
        printf("傳送內容: %s\n" ,str);
        printf("傳送內容長度: %d bytes\n\n", strlen(str)+1);
   }
   else{
        printf("傳送內容: \n");
        printf("傳送內容長度: 1 bytes\n\n");
   }

   if(input_num == 2){
        printf("回應內容: \n");
        printf("回應內容長度: 1 bytes\n");
   }
   else{
        n=recv(sd, str_r, MAXLINE, 0); //接收
        printf("回應內容: %s \n",str_r);
        printf("長度: %d\n", strlen(str_r));
        printf("回應內容長度: %d bytes\n", n);
   }

   closesocket(sd); //關閉TCP socket

   WSACleanup();  // 結束 WinSock DLL 的使用
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


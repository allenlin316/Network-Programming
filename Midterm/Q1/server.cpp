// simple TCP echo server
#include <stdio.h>
#include <winsock.h>
#include <iostream>
#define MAXLINE 1024
using namespace std;

string toString(char *c_str);
char* toCString(string str);

int main(){
	SOCKET sd,clnt_sd;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n;
	string str;
	char c_str[100];
	int total=0;

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sd, (struct sockaddr *) &serv, sizeof(serv));

	listen(sd,5);
	while(1){
        int clnt_len=sizeof(clnt);
        cout << "server waits.\n";
        clnt_sd =  accept(sd, (struct sockaddr *)  &clnt,&clnt_len );
        while(1){
            n = recv(clnt_sd, c_str, 100, 0);
            if(strcmp(c_str, "=") != 0)
                total = total + stoi(toString(c_str));
            if(n>0 && c_str[0]=='='){
                cout << "recv: " << "= send(Total): " << total << endl;
                send(clnt_sd, itoa(total, c_str, 10), strlen(itoa(total, c_str, 10))+1, 0);
                cout << "connection closed\n";
                total=0;
                break;
            }
            c_str[n] = '\0'; // 加上結束字符，才能順利印出字串(才不會有其他不該出現的字元)
            cout << "recv: " << c_str;
            cout << " send: ACK\n";
            strcpy(c_str, "ACK");
            send(clnt_sd, c_str, strlen(c_str)+1,0);
            if(n == 0)
                break;
        }
        closesocket(clnt_sd);
	}


	closesocket(sd);

	WSACleanup();

}

string toString(char *c_str){
    string str = c_str;
    return str;
}

char* toCString(string str){
    char *c_str = new char[MAXLINE];
    strcpy(c_str, str.c_str());
    return c_str;
}

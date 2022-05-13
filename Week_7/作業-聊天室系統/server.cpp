#include <winsock.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <cctype>
#include <vector>
#define MAXLINE 1024
using namespace std;
/*
    功能
        1. 三人交談
        2. 不雅字馬掉 (E.G. f**k)
        3. 當有人說bye，結束此聊天室
        4. client設定發言者名後，將來對話顯示發言者
*/

string toString(char *c_str);
char* toCString(string str);
void replaceBanWord(string &str);
bool closeChatRoomCheck(string str);

int main()
{
    SOCKET sd,clnt_sd0,clnt_sd1,clnt_sd2, clnt_sd3;
	WSADATA wsadata;
	struct sockaddr_in serv,clnt0,clnt1,clnt2, clnt3;
	int i,n;
	char c_str[MAXLINE];
	string str;

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sd, (struct sockaddr *) &serv, sizeof(serv));

	listen(sd,5);
	int clnt_len1=sizeof(clnt1);
	int clnt_len2=sizeof(clnt2);
	int clnt_len3 = sizeof(clnt3);
	cout << "Server waits.\n";
	clnt_sd1 =  accept(sd, (struct sockaddr *)  &clnt1,&clnt_len1 );
	cout << "(1) 1 connection.\n";
	clnt_sd2 =  accept(sd, (struct sockaddr *)  &clnt2,&clnt_len2 );
	cout << "(2) 2 connections.\n";
	clnt_sd3 = accept(sd, (struct sockaddr *) &clnt3, &clnt_len3);
	cout << "(3) 3 connections.\n";

	int clnt_len0=sizeof(clnt0);
    while(1){
        clnt_sd0 = accept(sd, (struct sockaddr *) &clnt0, &clnt_len0);
        n = recv(clnt_sd0, c_str, 100, 0);
        if(n < 0)   break;
        str = toString(c_str);
        replaceBanWord(str);
        if(closeChatRoomCheck(str)){
            cout << "chat room closed\n";
            str = "chat room closed";
            send(clnt_sd1, toCString(str), strlen(toCString(str))+1, 0);
            break;
        }
        closesocket(clnt_sd0);
        cout << "recv: " << str << endl;
        send(clnt_sd1, toCString(str), strlen(toCString(str))+1, 0);
        send(clnt_sd2, toCString(str), strlen(toCString(str))+1, 0);
        send(clnt_sd3, toCString(str), strlen(toCString(str))+1, 0);
    }
    closesocket(sd);
    closesocket(clnt_sd0);
    closesocket(clnt_sd1);
    closesocket(clnt_sd2);
    closesocket(clnt_sd3);
    WSACleanup();
    system("pause");
    return 0;
}

string toString(char *c_str){
    string str;
    str = c_str;
    return str;
}

char* toCString(string str){
    char *c_str = new char[MAXLINE];
    strcpy(c_str, str.c_str());
    return c_str;
}

void replaceBanWord(string &str){
    vector<string> banWords = {"fuck", "shit", "bullshit", "bitch"};
    int pch, ban_length;
    for(int i=0; i<banWords.size(); i++){
        pch = str.find(banWords[i]);
        ban_length = banWords[i].size() / 2;
        if(pch != -1){
            while(ban_length--){
                pch+=1;
                str[pch] = '*';
            }
        }
    }
}

bool closeChatRoomCheck(string str){
    for(int i=0; i<str.size(); i++){
        str[i] = tolower(str[i]);
    }
    int flag = str.find("bye");
    if(flag == -1)  return false;
    else    return true;
}

#include <iostream>
#include <sstream>
#include <vector>
#include <winsock.h>
using namespace std;

#define MAXLINE 1024

string toString(char *c_str);
char* toCString(string str);
bool closeChatRoomCheck(string str);

int main()
{
    SOCKET sd;
    struct sockaddr_in serv;
    WSADATA wsadata;
    char c_str[MAXLINE];
    string str, user;
    int n;

    WSAStartup(0x101, &wsadata);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(1234);
    cout << "Please type the user name: ";
    cin >> user;
    user = " [ from " + user + "]";
    cin.get(); //to prevent gets from access \n

    while(1){
        sd = socket(AF_INET, SOCK_STREAM, 0);
        cout << "Input: ";
        gets(c_str);
        strcat(c_str, toCString(user));
        connect(sd, (struct sockaddr *) &serv, sizeof(serv));
        send(sd, c_str, strlen(c_str)+1, 0);
        str = toString(c_str);
        cout << "Send: " << str << endl;
        if(closeChatRoomCheck(str)){
            cout << "chat room closed\n";
            break;
        }
        closesocket(sd);
    }
    closesocket(sd);
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

bool closeChatRoomCheck(string str){
    for(int i=0; i<str.size(); i++){
        str[i] = tolower(str[i]);
    }
    int flag = str.find("bye");
    if(flag == -1)  return false;
    else    return true;
}

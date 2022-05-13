#include <iostream>
#include <sstream>
#include <winsock.h>
using namespace std;

#define MAXLINE 1024

string toString(char *c_str);

int main()
{
    SOCKET sd;
    struct sockaddr_in serv;
    WSADATA wsadata;
    char c_str[MAXLINE];
    string str, user;
    int n;

    WSAStartup(0x101, &wsadata);
    sd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(1234);

    connect(sd, (struct sockaddr *) &serv, sizeof(serv));
    cout << "connect to server" << endl;

    while(1){
        n = recv(sd, c_str, MAXLINE, 0);
        if(n < 0)   break;
        if(n == 0){
            cout << "connection closed\n";
            break;
        }
        cout << "recv: " << toString(c_str) << endl;
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

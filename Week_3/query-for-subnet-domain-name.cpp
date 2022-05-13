#include <iostream>
#include <cstring>
#include <sstream>
#include <winsock.h>

using namespace std;

int main()
{
    char ip_str[32];
    string ip_divide="";
    WSADATA wsadata;
    int nResult = WSAStartup (0x101, (LPWSADATA) &wsadata);
    if(nResult != 0){
        cout << "WSA Initialization failed: " << nResult << endl;
        WSACleanup();
        return 0;
    }
    LPHOSTENT hp;
    struct in_addr sAddr;
    cout << "Print all of IP's domain names of subnet" << endl;
    cout << "Input subnet addr: ";
    cin >> ip_str;
    char *delim = ".";
    string pch;
    pch = strtok(ip_str, delim);
    for(int i=0; i<2; i++){
        ip_divide += pch;
        ip_divide+=".";
        pch = strtok(NULL, delim);
    }
    ip_divide += pch; // 把最後一個加回去，因為最後一個數字後面沒有 '.'
    for(int i=1; i<=5; i++){
        stringstream ss, ss2;
        string subnet, sub_ip_char_arr_string=""; char sub_ip_char_arr[32];
        ss << i;
        ss >> subnet;
        sub_ip_char_arr_string = ip_divide + "." + subnet;
        ss2 <<  sub_ip_char_arr_string; // to change to char array cuz the inet_addr()
        ss2 >> sub_ip_char_arr;
        sAddr.s_addr = inet_addr(sub_ip_char_arr); // turn string to 32-bit IP addr.
        hp = gethostbyaddr((LPSTR) &sAddr, sizeof(sAddr), AF_INET); // return host entity, including name, aliases...
        if(hp != NULL) // subnet has domain
            cout << sub_ip_char_arr_string << " domain name " << hp->h_name << endl;
    }
}

#include <stdio.h>
#include <winsock.h>

/*
    這個為聊天室第二個版本
    有中間 server 當兩個 client 的中繼站
*/

int main()
{
    SOCKET sd, clnt_sd1, clnt_sd2;
    struct sockaddr_in serv, clnt1, clnt2;
    int n;
    char str[100];
    //char accounts[2][6] = {"user1", "user2"};
    //char pwds[2][4] = {"abc","def"};
    int times=0;
    char usedUser[5];
    char check[10];
    WSADATA wsadata;

    WSAStartup(0x101, &wsadata);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(1234);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sd, (struct sockaddr *) &serv, sizeof(serv));

    listen(sd, 5);
    int clnt_len1 = sizeof(clnt1);
    int clnt_len2 = sizeof(clnt2);
    printf("Server waits.\n");
    clnt_sd1 = accept(sd, (struct sockaddr *) &clnt1, &clnt_len1);
    printf("One client connects\n");
    //printf("%s\n", pwds[0]);
    // 判斷 client1 帳密
    while(1){
        n = recv(clnt_sd1, str, 100, 0);
        if(n <= 0)   break;
        if(strcmp(str, "user1")==0 || strcmp(str, "user2")==0){ // it means account is valid
            times=0;
            strcpy(usedUser, str); // mark the username
            strcat(usedUser, "\0");
            printf("Client 1 username correct: %s\n", str);
            strcpy(str, "username correct");
            send(clnt_sd1, str, strlen(str)+1, 0);
            n = recv(clnt_sd1, str, 100, 0);
            if(strcmp(str, "abc")==0 || strcmp(str, "def")==0){
                printf("Client 1 password correct: %s\n", str);
                strcpy(str, "password correct");
                send(clnt_sd1, str, strlen(str)+1, 0);
                break;
            }
            else{
                times++;
                printf("Client 1 password incorrect: %s for %d times\n", str, times);
                strcpy(str, "password incorrect");
                send(clnt_sd1, str, strlen(str)+1, 0);
                if(times >= 3){
                    closesocket(sd);
                    closesocket(clnt_sd1);
                    exit(1);
                }
            }
        }
        else{
            times++;
            printf("Client 1 username incorrect: %s for %d times\n", str, times);
            strcpy(str, "username incorrect");
            send(clnt_sd1, str, strlen(str)+1, 0);
            if(times >= 3){
                closesocket(sd);
                closesocket(clnt_sd1);
                exit(1);
            }
        }
    }
    clnt_sd2 = accept(sd, (struct sockaddr *) &clnt2, &clnt_len2);
    times=0;
    while(1){
        n = recv(clnt_sd2, str, 100, 0);
        if(n <= 0)   break;
        if(strcmp(str, "user1")==0 || strcmp(str, "user2")==0){ // it means account is valid
            times=0;
            printf("Client 2 username correct: %s\n", str);
            strcpy(check, str);
            strcpy(str, "username correct");
            send(clnt_sd2, str, strlen(str)+1, 0);
            n = recv(clnt_sd2, str, 100, 0);
            if(strcmp(str, "abc")==0 || strcmp(str, "def")==0){
                printf("check: %s, usedUser: %s\n", check, usedUser);
                if(strcmp(check, usedUser)==0){
                    printf("This username has been used.\n");
                    closesocket(sd);
                    closesocket(clnt_sd1);
                    closesocket(clnt_sd2);
                    exit(1);
                }
                printf("Client 2 password correct: %s\n", str);
                strcpy(str, "password correct");
                send(clnt_sd2, str, strlen(str)+1, 0);
                break;
            }
            else{
                times++;
                printf("Client 2 password incorrect: %s for %d times\n", str, times);
                strcpy(str, "password incorrect");
                send(clnt_sd2, str, strlen(str)+1, 0);
                if(times >= 3){
                    closesocket(sd);
                    closesocket(clnt_sd2);
                    exit(1);
                }
            }
        }
         else{
            times++;
            printf("Client 2 username incorrect: %s for %d times\n", str, times);
            strcpy(str, "username incorrect");
            send(clnt_sd2, str, strlen(str)+1, 0);
            if(times >= 3){
                closesocket(sd);
                closesocket(clnt_sd2);
                exit(1);
            }
        }
    }
    printf("Two clients connect.\n");

    while(1){
        n = recv(clnt_sd1, str, 100, 0);
        printf("1->2: %s\n", str);
        send(clnt_sd2, str, strlen(str)+1, 0);
        n = recv(clnt_sd2, str, 100, 0);
        //str[n] = '\0';
        if(n <= 0)   break;
        printf("2->1: %s\n", str);
        send(clnt_sd1, str, strlen(str)+1, 0);
    }
    closesocket(sd);
    closesocket(clnt_sd1);
    closesocket(clnt_sd2);
    WSACleanup();
}

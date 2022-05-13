#include <stdio.h>
#include <time.h>

/*
    How many times does the while loop run for 1 sec
*/

int main()
{
    time_t start, in_loop;
    time(&start); // set the initial time
    int cnt=0;
    while(1){
        Sleep(100);
        printf("......\n");
        cnt++;
        time(&in_loop); // everytime run the loop will get the current time
        if(difftime(in_loop, start) > 1.0){ // how many times does the while loop run for 1 sec
            printf("the loop ran %d times", cnt);
            break;
        }
    }
    return 0;
}

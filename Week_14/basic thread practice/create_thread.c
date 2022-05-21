#include <stdio.h>
#include <winsock.h>

void *ThreadMain(void); // Main program of thread

int main()
{
    int threadID;                 /* Thread ID from CreateThread() */
	printf("Hello World from main()! \n");
    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, NULL, 0, (LPDWORD) &threadID) == NULL)
        printf("CreateThread() failed");
	printf("Thread %ld created.\n", threadID);
	Sleep(1000);
	return 0;
}

void *ThreadMain(void) //
{
	printf("Hello World from Thread! \n");
}

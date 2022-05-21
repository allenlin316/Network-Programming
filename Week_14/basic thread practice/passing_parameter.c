#include <stdio.h>
#include <winsock.h>

void *ThreadMain(void *arg);             /* Main program of a thread */

/* Structure of arguments to pass to client thread */
struct ThreadArgs
{
    int c;
};

int main(int argc, char *argv[])
{
	struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
    int  threadID;                 /* Thread ID from CreateThread() */

    threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
	threadArgs -> c = 5;

	printf("Hello World from main() pass:%d\n",threadArgs -> c);



    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs, 0, (LPDWORD) &threadID) == NULL)
        printf("CreateThread() failed");

	printf("Thread %d created.\n", threadID);
	Sleep(1000);
	return 0;
}

void *ThreadMain(void *threadArgs)
{
	int arg;

    arg = ((struct ThreadArgs *) threadArgs) -> c;
	printf("Hello World from Thread with arg: %d\n",arg);
	free(threadArgs);              /* Deallocate memory for argument */
}


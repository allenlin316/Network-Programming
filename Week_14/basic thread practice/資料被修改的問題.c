#include <stdio.h>
#include <winsock.h>

void *ThreadMain1(void *arg);             /* Main program of a thread */
void *ThreadMain2(void *arg);             /* Main program of a thread */

/* Structure of arguments to pass to client thread */
struct ThreadArgs
{
    int a;
    int b;
    int c;
};

int main()
{
	struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
    int  threadID1,threadID2;                 /* Thread ID from CreateThread() */

    threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
	threadArgs -> a = 5;
	threadArgs -> b = 3;

	printf("Hello World from main()\n");

    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain1, threadArgs, 0, (LPDWORD) &threadID1) == NULL)
        printf("CreateThread() failed");

	printf("Thread %d created.\n", threadID1);
	Sleep(1000);

    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain2, threadArgs, 0, (LPDWORD) &threadID2) == NULL)
        printf("CreateThread() failed");

	printf("Thread %d created.\n", threadID2);
	Sleep(2000);

	free(threadArgs);              /* Deallocate memory for argument */
	return 0;
}

void *ThreadMain1(void *threadArgs)
{
	int i,j,k;

    i = ((struct ThreadArgs *) threadArgs) -> a;
    j = ((struct ThreadArgs *) threadArgs) -> b;
    k = i + j;
	((struct ThreadArgs *) threadArgs) -> c = k;
	Sleep(2000); // 這樣會被後面的 thread 覆蓋過去
	printf("%d + %d = %d \n",i,j,((struct ThreadArgs *) threadArgs) -> c);
}

void *ThreadMain2(void *threadArgs)
{
	int i,j,k;

    i = ((struct ThreadArgs *) threadArgs) -> a;
    j = ((struct ThreadArgs *) threadArgs) -> b;
    k = i - j;
	((struct ThreadArgs *) threadArgs) -> c = k;
	printf("%d - %d = %d \n",i,j,k);
}

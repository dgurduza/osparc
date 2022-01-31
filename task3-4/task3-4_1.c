#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t thisID = getpid();
    pid_t parentID = getppid();
    pid_t forkedID = fork();

    printf("PID = %d\nPPID = %d\n", thisID, parentID);

    if (forkedID < 0) {
        printf("Problems with forking process (%d)\n", forkedID);
    } else if (forkedID > 0) {
        printf("Parent process. (%d)\n", forkedID);
    } else {
        printf("Child process. (%d)\n", forkedID);
    }
    return 0;
}
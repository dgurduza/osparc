#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

void event() {
    int status;
    pid_t pid;

    pid = waitpid(-1, &status, WNOHANG);
    while (pid) {
        if (pid < 0) {
            if (errno == 10)
                printf("Все дочерние закончили работы\n");
            else
                printf("ОШИБКА: %d\n", errno);

            return;
        } else {
            if ((status & 0xff) == 0) {
                printf("Процесс %d закончен со статусом %d\n", pid, status >> 8);
            } else if ((status & 0xff00) == 0) {
                printf("Процесс %d убит сигналом %d %s\n", pid, status & 0x7f,
                       (status & 0x80) ? "с core" : "без core");
            }
            pid = waitpid(-1, &status, WNOHANG);
        }
    }
}

int main(void) {
    pid_t pid;
    int i, j;

    (void) signal(SIGCHLD, event);

    for (i = 0; i < 10; i++) {
        if ((pid = fork()) < 0) {
            printf("Can\'t fork child 1\n");
            exit(1);
        } else if (pid == 0) {
            for (j = 1; j < 10000000; j++);
            exit(200 + i);
        }
    }

    while (1);
    return 0;
}
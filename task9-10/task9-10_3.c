#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int D(int sem, struct sembuf *buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(sem, buf, 1);
}

int A(int sem, struct sembuf *buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(sem, buf, 1);
}

int main() {
    int N, i;
    printf("Enter count of iterations (>2):\n> ");
    scanf("%d", &N);
    if (N < 2) {
        printf("Incorrect! N = 2\n");
        N = 2;
    }

    char pathname[] = "task9-10_3.c";
    key_t key;
    int semid;
    struct sembuf mybuf;
    int fd[2], result;
    int size;
    char resstring[16];

    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t create semaphore set\n");
            exit(-1);
        }
        printf("Success\n");
    }
    result = fork();

    for (i = 0; i < N; ++i) {
        if (result < 0) {
            printf("Can\'t fork child[%d]\n", i);
            exit(-1);
        } else if (result > 0) {
            printf("Parent[%d]: start\n", i);

            size = write(fd[1], "Hello, world![p]", 16);
            if (size != 16) {
                printf("Parent[%d]: can\'t write all string to pipe\n", i);
                exit(-1);
            }
            printf("Parent[%d]: wrote message to child\n", i);

            D(semid, &mybuf);
            size = read(fd[0], resstring, 16);

            if (size < 0) {
                printf("Parent[%d]: can\'t read string from pipe\n", i);
                exit(-1);
            }
            printf("Parent[%d]: read message from child\n> %s\n", i, resstring);
            printf("Parent[%d]: end\n", i);
        } else {
            printf("Child[%d]: start\n", i);

            size = read(fd[0], resstring, 16);
            if (size < 0) {
                printf("Child[%d]: can\'t read string from pipe\n", i);
                exit(-1);
            }
            printf("Child[%d]: read message from parent\n> %s\n", i, resstring);

            size = write(fd[1], "Hello, world![ñ]", 16);
            if (size != 16) {
                printf("Child[%d]: can\'t write all string to pipe\n", i);
                exit(-1);
            }
            printf("Child[%d]: wrote message to parent\n", i);

            A(semid, &mybuf);
            printf("Child[%d]: end\n", i);
        }
    }

    close(fd[0]);
    close(fd[1]);

    return 0;
}
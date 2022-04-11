#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
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
    int *array;

    int new = 1;
    long i;

    int shmid;
    int semid;
    char pathname[] = "task9-10_2_1.c";
    
    key_t key;
    struct sembuf mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Can\'t create shared memory\n");
            exit(-1);
        } else {
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
            new = 0;
        }
    }

    if ((array = (int *) shmat(shmid, NULL, 0)) == (int *) (-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t create semaphore set\n");
            exit(-1);
        }
        
        printf("Success\n");
        
        A(semid, &mybuf);
    }

    if (new) {
        array[0] = 1;
        array[1] = 0;
        array[2] = 1;
    } else {
        D(semid, &mybuf);

        array[0] += 1;
        for (i = 0; i < 2000000000L; i++) ;
        array[2] += 1;

        A(semid,&mybuf);
    }
    
    printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
           array[0], array[1], array[2]);

    if (shmdt(array) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    return 0;
}
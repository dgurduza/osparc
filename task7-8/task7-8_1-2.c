#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    int shmid, file_size, new = 1;
    char path[] = "task7-8_1-1.c";
    key_t key;
    char *text;
    int *size;

    FILE *file;
    file = fopen(path, "r");
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    fclose(file);

    if ((key = ftok(path, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((shmid = shmget(key, sizeof(int), 0666)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
    }

    if ((size = (int *) shmat(shmid, NULL, 0)) == (int *) (-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    text = (char *) (size + 1);

    for (int i = 0; i < *size; i++) {
        putchar(text[i]);
    }

    if (shmdt(size) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        printf("Can't delete shared memory\n");
        exit(-1);
    }
    return 0;
}
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    int msqid;
    key_t key;
    char pathname[] = "task11-12_3_1.c";
    int len, maxlen;

    struct msgbuf {
        long mtype;
        struct {
            pid_t pid;
            float finfo;
        } info;
    } mybuf;

    struct servmsgbuf {
        long mtype;
        float finfo;
    } myservbuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    while (1) {
        maxlen = sizeof(mybuf.info);
        if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 1, 0)) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        printf("Request from client %d\n", mybuf.info.pid);
        myservbuf.mtype = mybuf.info.pid;
        myservbuf.finfo = mybuf.info.finfo * mybuf.info.finfo;
        len = sizeof(myservbuf.finfo);
        if (msgsnd(msqid, (struct servmsggbuf *)&myservbuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(-1);
        }

        printf("\nResponded\n");
    }

    return 0;
}
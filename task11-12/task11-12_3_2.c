#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int msqid;
    pid_t getpid(void);
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

    mybuf.mtype = 1;
    mybuf.info.pid = getpid();
    printf("Enter float number: ");
    scanf("%f", &(mybuf.info.finfo));
    len = sizeof(mybuf.info);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Message from %d was sent.\n", getpid());
    maxlen = sizeof(myservbuf.finfo);

    if ((len = msgrcv(msqid, &myservbuf, maxlen, getpid(), 0)) < 0) {
        printf("Can't receive message from queue\n");
        exit(-1);
    }

    printf("Response:\t%f\n", myservbuf.finfo);

    return 0;
}
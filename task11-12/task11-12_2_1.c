#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void) {
    int msqid;
    key_t key;
    char pathname[] = "task11-12_2_1.c";
    int len, maxlen;

    struct mymsgbuf {
        long mtype;
        struct {
            char text[23];
            int num;
        } info;
    } mybuf;
    
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    printf("\'task11-12_2_1.c\' start sending messages\n");

    for (int i = 0; i < 5; i++) {
        mybuf.mtype = 1;
        mybuf.info.num = i;
        strcpy(mybuf.info.text, "Message from script 1.");
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }

        printf("Message %d was sent with info = %d ; \'%s\'\n", i, mybuf.info.num, mybuf.info.text);
    }
    printf("\'task11-12_2_1.c\' finished sending messages\n");
    printf("\n");

    printf("\'task11-12_2_1.c\' started receiving messages\n");

    for (int i = 0; i < 5; i++) {
        maxlen = sizeof(mybuf.info);

        if ((len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        printf("> message type = %ld, info = %d ; \'%s\'\n", mybuf.mtype, mybuf.info.num, mybuf.info.text);
    }

    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

    printf("\'task11-12_2_1.c\' finished sending messages\n");

    return 0;
}
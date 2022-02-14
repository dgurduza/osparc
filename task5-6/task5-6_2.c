#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int ch_to_pr_fd[2], 
        pr_to_ch_fd[2], 
        result;

    size_t size;
    char resstring[14];
    char second_resstring[18];

    if (pipe(pr_to_ch_fd) < 0) {
        printf("Can\'t open parent to child pipe\n");
        exit(-1);
    }

    if (pipe(ch_to_pr_fd) < 0) {
        printf("Can\'t open parent to child pipe\n");
        exit(-1);
    }

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {

        /* Parent process */

        if (close(pr_to_ch_fd[0]) < 0) {
            printf("Parent: Can\'t close reading side of pipe\n");
            exit(-1);
        }

        size = write(pr_to_ch_fd[1], "Hello, child!", 14);

        if (size != 14) {
            printf("Can\'t write all string to pipe\n");
            exit(-1);
        }

        if (close(pr_to_ch_fd[1]) < 0) {
            printf("Parent: Can\'t close writing side of pipe\n");
            exit(-1);
        }

        printf("Parent wrote message\n");
        printf("Parent exit\n");

    } else {

        /* Child process */

        if (close(pr_to_ch_fd[1]) < 0) {
            printf("Child: Can\'t close writing side of pipe\n");
            exit(-1);
        }

        size = read(pr_to_ch_fd[0], resstring, 14);

        if (size < 0) {
            printf("Can\'t read string from pipe\n");
            exit(-1);
        }

        printf("Child exit, resstring:%s\n", resstring);

        if (close(pr_to_ch_fd[0]) < 0) {
            printf("Child: Can\'t close reading side of pipe\n");
            exit(-1);
        }

        if (close(ch_to_pr_fd[0]) < 0) {
            printf("Child: can\'t close reading side of pipe\n");
            exit(-1);
        }

        size = write(ch_to_pr_fd[1], "Hello, world!", 14);

        if (size != 14) {
            printf("Child: can\'t write all string to pipe\n");
            exit(-1);
        }

        if (close(ch_to_pr_fd[1]) < 0) {
            printf("Child: can\'t close writing side of pipe\n");
            exit(-1);
        }

        printf("Child: wrote message\n");
        printf("Child: exit\n");
    }

    sleep(1);

    if (result > 0) {

        /* Parent process */

        if (close(ch_to_pr_fd[1]) < 0) {
            printf("Parent: can\'t close writing side of pipe\n");
            exit(-1);
        }

        size = read(ch_to_pr_fd[0], second_resstring, 14);

        if (size < 0) {
            printf("Parent: can\'t read string from pipe\n");
            exit(-1);
        }

        printf("Parent exit, resstring:%s\n", second_resstring);

        if (close(ch_to_pr_fd[0]) < 0) {
            printf("Parent: can\'t close reading side of pipe\n");
            exit(-1);
        }

        printf("Parent: exit\n");
    }

    return 0;
}
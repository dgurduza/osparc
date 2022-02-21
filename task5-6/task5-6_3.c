#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int size, result_size;
    int fd[2];
    char* resstring = malloc(1 * sizeof(char));

    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    do {
        size = write(fd[1], resstring, 1);
        ++result_size;
        printf("%d\n", result_size);
    } while (size == 1);
    --result_size;

    printf("Result pipe size: %d", result_size);

    return 0;
}
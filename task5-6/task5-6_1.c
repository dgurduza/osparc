#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    size_t word_size = 14;
    int fd;
    char string[word_size];
    char *file_name = "file";

    (void)umask(0);

    if ((fd = open(file_name, O_RDONLY)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }

    if (read(fd, string, word_size) != word_size) {
        printf("Can\'t read file\n");
        exit(-1);
    }

    printf("File: %s\nText in file:\n%s\n Bytes: %ld\n", file_name, string, word_size);

    if (close(fd) < 0) {
        printf("Can\'t close file\n");
        exit(-1);
    }

    return 0;
}
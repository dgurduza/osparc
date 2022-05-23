#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    const char* folder = "/temp";
    struct stat sb;
    int fd;
    char *filename = "1.txt";

    if (stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        system("rm -r temp");
    }
    system("mkdir temp");
    chdir("./temp");

    if ((fd = open(filename, O_CREAT | O_WRONLY)) == -1) {
        printf("Can\'t create file!\n");
        exit(-1);
    }

    if (close(fd) < 0) {
        printf("Can\'t close file!\n");
        exit(-1);
    }

    char *file = malloc(1000);
    char *link = malloc(1000);
    int depth = 1;

    while (1) {
        sprintf(file, "%d.txt", depth);
        sprintf(link, "%d.txt", depth + 1);
        if (symlink(file, link) == 0) {
            if ((fd = open(link, O_RDONLY)) < 0) {
                printf("Result: %d\n", depth - 1);
                exit(-1);
            }
            if (close(fd) < 0) {
                printf("Can\'t close link %s!\n", link);
                exit(-1);
            }
        } else {
            printf("Can\'t create link for file %s\n", file);
            exit(-1);
        }
        depth++;
    }

    return 0;
} 
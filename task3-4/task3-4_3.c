#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    int index = 0;
    for (int j = 0; j < argc; ++j) {
        printf("Arguments[%d] = %s\n", j, argv[j]);
    }

    printf("\n");
    while (envp[index] != NULL) {
        printf("Params[%d] = %s\n", index, envp[index]);
        ++index;
    }
    return 0;
}
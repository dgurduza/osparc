#include <stdio.h>
#include <unistd.h>

int main() {
    printf("User ID: %u.\nGroup ID:%u.\n", getuid(), getgid());
    return 0;
}
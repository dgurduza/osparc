#include <signal.h>
#include <stdio.h>

void event(int signal_num) {
    switch (signal_num) {
        case 2:
            printf("CTRL + C were pressed\n");
            printf("Signal: [%d]\n", signal_num);
            break;
        case 3:
            printf("CTRL + 4 were pressed\n");
            printf("Signal: [%d]\n", signal_num);
            break;
    }
}

int main(void) {
    (void)signal(SIGINT, event);

    (void)signal(SIGQUIT, event);

    while(1);
    return 0;
}
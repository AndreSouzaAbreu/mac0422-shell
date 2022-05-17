#include <lib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    int option, priority, i;
    pid_t childp = 0;


    while (scanf("%d %d", &option, &priority) > 0) {
        switch (option) {
            case 0:
                printf("unlockpriority\n");
                unlockpriority(childp);
                break;
            case 1:
                printf("lockpriority\n");
                printf("Status: %d\n", lockpriority(childp, priority));
                break;
            case 2:
                if ((childp = fork()) == 0) {
                    printf("Child hello\n");
                    sleep(5);
                    for (i = 0; i < 2000000000; i++)
                        printf("");
                    for (i = 0; i < 2000000000; i++)
                        printf("");
                    for (i = 0; i < 2000000000; i++)
                        printf("");
                    printf("Child bye\n");
                    return 0;
                } 
                break;
        }
    }

    return 0;
}

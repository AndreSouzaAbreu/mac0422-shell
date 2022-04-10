#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main () {
    int a = 0, b = 1;
    sleep(10);
    scanf("%d", &a);
    printf("%d %d\n", b, a);
    sleep(10);
    return 0;
}

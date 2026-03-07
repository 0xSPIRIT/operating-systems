#include <stdio.h>
#include <unistd.h>

int main(void) {
    fork();
    fork();
    printf("Welcome Message\n"); fflush(stdout);
}

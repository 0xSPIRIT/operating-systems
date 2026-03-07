#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  pid_t pid;

  printf("This line is printed before the call to fork()\n");
  fflush(stdout);

  pid = fork();

  printf("This line is printed after the call to fork()\n");
  fflush(stdout);
}

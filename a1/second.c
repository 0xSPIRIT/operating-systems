#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_func(int a) {
  if (a <= 0)
    return;

  pid_t pid;

  if ((pid = fork())) {
    printf("Created Child, proccess id = %d\n", pid);
    fflush(stdout);
  }

  child_func(a - 1);

  wait(NULL);
}

int main(void) {
  child_func(7);
  return 0;
}

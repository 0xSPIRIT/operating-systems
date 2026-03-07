#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void fib(pid_t child_pid) {
  int x = 0, y = 1, v;

  for (int i = 0; i < 10; i++) {
    if (i < 2) {
      v = i;
    } else {
      v = x + y;
      x = y;
      y = v;
    }

    printf("Child Process ID: %d   Fibonacci Sequence: %d\n", child_pid, v);
    fflush(stdout);
  }
}

int main(void) {
  printf("Parent Process ID: %d\n\n", getpid());

  pid_t child_pid = fork();

  if (!child_pid) {
    pid_t pid = getpid(); // Get the child pid

    if (pid % 5 == 0) {
      fib(pid);
    } else {
      printf("Child Process ID: %d   Child Process ID is not divisible by 5.\n", pid);
    }
  }

  wait(NULL);
}

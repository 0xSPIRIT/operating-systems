#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum;

void *runner(void *param) {
  int value = atoi((char *)param);
  sum = value * (value + 1) / 2;

  pthread_exit(0);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
    return 1;
  }

  pthread_t tid;
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  if (pthread_create(&tid, &attr, runner, argv[1])) {
    fprintf(stderr, "Error creating the pthread!\n");
    return 1;
  }

  pthread_join(tid, NULL);

  printf("Sum = %d\n", sum);

  return 0;
}

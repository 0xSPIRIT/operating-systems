#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct {
  int fact, square, half;
  bool is_odd;
} global_result = {0};

void *job_factorial(void *arg) {
  int n = *(int *)arg;

  int result = 1;

  if (n > 1)
    for (int k = 2; k <= n; k++)
      result *= k;

  global_result.fact = result;

  pthread_exit(NULL);
}

void *job_square(void *arg) {
  int n = *(int *)arg;

  global_result.square = n * n;

  pthread_exit(NULL);
}

void *job_half(void *arg) {
  int n = *(int *)arg;

  if (n % 2 == 0) {
    global_result.half = n / 2;
  } else {
    global_result.is_odd = true;
  }

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  if (argc != 4) {
    // Incorrect usage
    fprintf(stderr, "Usage: %s n1 n2 n3\n", argv[0]);
    return 1;
  }

  int n1 = atoi(argv[1]);
  int n2 = atoi(argv[2]);
  int n3 = atoi(argv[3]);

  pthread_t worker_fact, worker_square, worker_even;

  pthread_create(&worker_fact, NULL, job_factorial, &n1);
  pthread_create(&worker_square, NULL, job_square, &n2);
  pthread_create(&worker_even, NULL, job_half, &n3);

  pthread_join(worker_fact, NULL);
  pthread_join(worker_square, NULL);
  pthread_join(worker_even, NULL);

  printf("The factorial of %d is %d\n", n1, global_result.fact);
  printf("The square of %d is %d\n", n2, global_result.square);

  if (global_result.is_odd)
    printf("The value is ODD\n");
  else
    printf("The half value of %d is %d\n", n3, global_result.half);

  return 0;
}

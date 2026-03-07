#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3

int balls;
int even_removed[NUM_THREADS];
int odd_removed[NUM_THREADS];
sem_t lock;

void *run_job(void *arg) {
  int i = *(int *)arg;

  while (1) {
    sem_wait(&lock);

    // critical section
    if (balls <= 0) {
      sem_post(&lock);
      break;
    }

    int b = --balls;

    if (b == 0) {
      printf("The last ball was removed by Thread %d\n", i);
    }

    printf("Thread %d removed ball %d. Balls remaining: %d\n", i, b, balls);

    fflush(stdout);

    if (b % 2 == 0) {
      even_removed[i]++;
    } else {
      odd_removed[i]++;
    }

    sem_post(&lock);
  }

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Error: Usage: %s (even number)\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);

  if (n % 2 != 0) {
    fprintf(stderr, "Error: You must pass an EVEN value\n");
    return 1;
  }

  balls = n;
  sem_init(&lock, 0, 1);

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_t threads[NUM_THREADS];
  int arr[] = {0, 1, 2};

  for (int i = 0; i < NUM_THREADS; i++) {
    printf("Creating thread %d\n", i);
    pthread_create(threads + i, &attr, run_job, arr + i);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    printf("Thread %d removed %d odd balls and %d even balls.\n", i,
           odd_removed[i], even_removed[i]);
  }

  printf("All balls have been removed.\n");

  return 0;
}

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *values;
  int count;
} Thread_Data;

float average;
int min, max;

void *calc_average(void *p_data) {
  Thread_Data *data = p_data;

  average = 0;

  for (int i = 0; i < data->count; i++) {
    average += data->values[i];
  }

  average /= data->count;

  pthread_exit(NULL);
}

void *calc_minimum(void *p_data) {
  Thread_Data *data = p_data;

  min = INT_MAX;

  for (int i = 0; i < data->count; i++) {
    if (data->values[i] < min)
      min = data->values[i];
  }

  pthread_exit(NULL);
}

void *calc_maximum(void *p_data) {
  Thread_Data *data = p_data;

  max = INT_MIN;

  for (int i = 0; i < data->count; i++) {
    if (data->values[i] > max)
      max = data->values[i];
  }

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  Thread_Data data;

  data.values = malloc(sizeof(int) * (argc - 1));
  data.count = argc - 1;

  for (int i = 0; i < data.count; i++) {
    data.values[i] = atoi(argv[i + 1]);
  }

  pthread_t thread_avg, thread_min, thread_max;

  pthread_create(&thread_avg, NULL, calc_average, &data);
  pthread_create(&thread_min, NULL, calc_minimum, &data);
  pthread_create(&thread_max, NULL, calc_maximum, &data);

  pthread_join(thread_avg, NULL);
  pthread_join(thread_min, NULL);
  pthread_join(thread_max, NULL);

  printf("The average value is %f. The minimum value is %d and the maximum "
         "value is %d\n",
         average, min, max);

  return 0;
}

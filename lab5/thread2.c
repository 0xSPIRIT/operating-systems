#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long long sum;

void *runner(void *param);

#define NUM 10

int main(int argc, char *argv[]) {
    pthread_t multi[NUM];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for(int i = 0; i < NUM; i++) {
        pthread_create(&multi[i], &attr, runner, argv[1]);
    }

    for(int i = 0; i < NUM; i++) {
        pthread_join(multi[i], NULL); 
        printf("Thread %d finished. sum = %lld\n", i, sum); 
    }
}

void *runner(void *param) {
    long i, upper = atoll(param);
    // RACE CONDITION HERE: All threads are modifying global sum at once
    sum = 0; 
    for (i = 1; i <= upper; i++) {
        sum += i;
    }
    pthread_exit(0);
}

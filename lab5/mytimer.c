#include <stdio.h>
#include <sys/time.h>

int main(void) {
  struct itimerval v;

  // Reset the timer to this when it expires
  v.it_interval.tv_sec = 5;
  v.it_interval.tv_usec = 0;

  // Set the current value of the timer to this
  v.it_value.tv_sec = 5;
  v.it_value.tv_usec = 0;

  setitimer(ITIMER_REAL, &v, NULL);

  for (int i = 0; i < 10000; i++) {
    for (long j = 0; j < 9999999; j++);

    if (!getitimer(ITIMER_REAL, &v)) {
      printf("%ld seconds, %ld microseconds\n", v.it_value.tv_sec, v.it_value.tv_usec);
      fflush(stdout);
    }
  }
}

// NOTE:
//
// When compiling with -std=c99, signal() behaviour changes and
// is reset to the default handler action after each caught alarm
// signal.
//
// However, when compiling with `gcc fib.c` with an up to date
// gcc version, it's fine.

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

// Enum to identify all the timers in the program (including KERN_TIMER)
typedef enum { REAL_TIMER, PROF_TIMER, VIRT_TIMER, KERN_TIMER } Timer_Type;

// Four microsecond-precise time values.
struct timeval real_time, prof_time, virt_time, kern_time;

// Slow recursive fib function
long long fib(long long n) {
  if (n <= 2)
    return 1;

  return fib(n - 1) + fib(n - 2);
}

// Alarm handler hook for the three timers
void handle_alarm(int sig) {
  switch (sig) {
    case SIGALRM:   real_time.tv_sec++; break;
    case SIGVTALRM: virt_time.tv_sec++; break;
    case SIGPROF:   prof_time.tv_sec++; break;
  }
}

// Stops a particular timer and stores the elapsed microseconds in the
// appropriate variable
void stop_timer(int which) {
  struct itimerval v;

  getitimer(which, &v);

  // Reset the timer
  struct itimerval zero_timer = {0};
  setitimer(which, &zero_timer, NULL);

  // Note that v.it_value represents the time remaining in
  // the timer. To get the elapsed time, we must subtract
  // from 1,000,000

  time_t usec_elapsed;

  if (v.it_value.tv_sec == 1) {
    usec_elapsed = 0;
  } else {
    usec_elapsed = 1000000 - v.it_value.tv_usec;
  }

  struct timeval *value = NULL;

  // A simple switch to get the right timer

  switch (which) {
    case ITIMER_REAL:    value = &real_time; break;
    case ITIMER_VIRTUAL: value = &virt_time; break;
    case ITIMER_PROF:    value = &prof_time; break;
    default:
      printf("Error! Incorrect timer.\n");
      exit(1);
  }

  value->tv_usec = usec_elapsed;
}

void print_timer_result(Timer_Type timer_type) {
  struct timeval *value = NULL;

  switch (timer_type) {
    case REAL_TIMER:
      value = &real_time;
      printf("Wall Time Spent Executing: ");
      break;
    case PROF_TIMER:
      value = &prof_time;
      printf("Total Time Spent on CPU: ");
      break;
    case VIRT_TIMER:
      value = &virt_time;
      printf("Time Spent in User Space: ");
      break;
    case KERN_TIMER:
      value = &kern_time;
      printf("Time Spent in Kernel Space: ");
      break;
    default:
      printf("Error! Incorrect timer.\n");
      exit(1);
  }

  printf("%ld seconds, %ld microseconds\n", value->tv_sec, value->tv_usec);
}

void start_timer(int which) {
  struct itimerval timer;

  // Declare a timer starting at 1s 0us, and that resets to 1s 0us.
  timer.it_value = timer.it_interval = (struct timeval){1, 0};

  // Sets the process interval timer
  setitimer(which, &timer, NULL);
}

// This is what runs in each process. It inputs the n'th fibonacci number
// to calculate and outputs it along with the performance metrics.
//
// if child_no == -1, then we output as the parent.
void run_fib_trial(int child_no, int n) {
  // Set up our alarm handler
  signal(SIGALRM,   handle_alarm);
  signal(SIGVTALRM, handle_alarm);
  signal(SIGPROF,   handle_alarm);

  start_timer(ITIMER_REAL);
  start_timer(ITIMER_VIRTUAL);
  start_timer(ITIMER_PROF);

  // Perform the intensive fib calculation
  long long result = fib(n);

  stop_timer(ITIMER_REAL);
  stop_timer(ITIMER_VIRTUAL);
  stop_timer(ITIMER_PROF);

  if (child_no == -1) {
    while (wait(NULL) > 0); // The parent waits on all its children to die.
    printf("[Parent] ");
  } else {
    printf("[Child %d] ", child_no);
  }

  printf("fibonacci(%d) = %lld\n", n, result);

  // Calculate the kernel time as the difference between the profile time and virtual time

  kern_time.tv_sec  = prof_time.tv_sec  - virt_time.tv_sec;
  kern_time.tv_usec = prof_time.tv_usec - virt_time.tv_usec;

  if (kern_time.tv_usec < 0) {
    kern_time.tv_sec--;
    kern_time.tv_usec += 1000000;
  }

  print_timer_result(REAL_TIMER);
  print_timer_result(PROF_TIMER);
  print_timer_result(VIRT_TIMER);
  print_timer_result(KERN_TIMER);

  printf("\n");

  fflush(stdout);
}

int main(int argc, char **argv) {
  if (argc != 4) {
    // Incorrect usage
    printf("Usage: %s n1 n2 n3\n", argv[0]);
    return 1;
  }

  const int count = 3;

  // Loop from i=1 to i=2, leaving i=3 for the parent to do after.
  for (int i = 1; i < count; i++) {
    int n = atoi(argv[i]);

    pid_t pid = fork();

    if (pid == -1) {
      fprintf(stderr, "Failed to fork! Aborting...\n");
      return 1;
    }

    if (!pid) {
      // Execute child job
      run_fib_trial(i, n);
      return 0; // Kill child and exit
    }
  }

  // Final calculation
  int n = atoi(argv[count]);
  run_fib_trial(-1, n);

  return 0;
}

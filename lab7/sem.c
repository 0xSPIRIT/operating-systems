#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>

#define SEMKEY 1234567

int main() {
  int semid, semval, i;
  struct sembuf sb;

  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = 1;

  // create a semophore
  semid = semget(SEMKEY, 1, 0600 | IPC_CREAT | IPC_EXCL);
  printf("The value of semget = %d\n", semid);

  // set value of semophore to 5
  semctl(semid, 0, SETVAL, 5);

  // get the value of the semophore
  semval = semctl(semid, 0, GETVAL, NULL);
  printf("The value of semval = %d\n", semval);

  for (i = 0; i < 6; i++) {
    // get the value of the semophore
    semval = semctl(semid, 0, GETVAL, NULL);
    printf("The value of semval = %d\n", semval);
  }

  semop(semid, &sb, 1);

  // remove a semophore
  semctl(semid, 0, IPC_RMID, NULL);

  return 1;
}

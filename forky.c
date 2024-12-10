#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// (just directly input ./forky 7 1... results1.txt)

// sleep seed for 1 to 8
void random_sleep() { sleep(rand() % 8 + 1); }

// pattern 1
void pattern1(int num_procs) {
  srand(time(NULL));
  for (int i = 1; i <= num_procs; i++) {
    if (fork() == 0) {
      printf("Process %d (PID %d) starting\n", i, getpid());
      random_sleep();
      printf("Process %d (PID %d) exiting\n", i, getpid());
      exit(0);
    } else {
      printf("Parent created Process %d\n", i);
    }
  }

  while (wait(NULL) > 0)
    ;
}

// pattern 2
void pattern2(int num_procs) {
  srand(time(NULL));
  for (int i = 1; i <= num_procs; i++) {

    if (fork() == 0) {
      printf("Process %d (PID %d) starting\n", i, getpid());
      random_sleep();
      printf("Process %d (PID %d) exiting\n", i, getpid());
      exit(0);
    } else {
      wait(NULL);
    }
  }
}

// pattern 3
void fork_tree(int level, int max_level) {
  if (level > max_level)
    return;

  if (fork() == 0) {
    printf("Process %d (PID %d) starting\n", level, getpid());
    random_sleep();

    fork_tree(2 * level, max_level);

    fork_tree(2 * level + 1, max_level);
    printf("Process %d (PID %d) exiting\n", level, getpid());
    exit(0);
  }

  wait(NULL); // waiting for lc
  wait(NULL); // rc
}

void pattern3(int num_procs) {
  srand(time(NULL));
  fork_tree(1, num_procs);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <num_processes> <pattern>\n", argv[0]);
    return 1;
  }

  int num_procs = atoi(argv[1]);
  int pattern = atoi(argv[2]);

  if (num_procs < 1 || num_procs > 256) {
    printf("Number of processes must be between 1 and 256.\n");
    return 1;
  }

  if (pattern == 1) {
    pattern1(num_procs);
  } else if (pattern == 2) {
    pattern2(num_procs);
  } else if (pattern == 3) {
    pattern3(num_procs);
  } else {
    printf("Invalid pattern. Use 1, 2, or 3.\n");
    return 1;
  }

  return 0;
}

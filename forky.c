#include <stdio.h>
#include <unistd.h>// for fork(), getpid, and sleep
#include <sys/wait.h>//  waitpid()
#include <stdlib.h>   
#include <time.h>  

// (just directly input ./forky 7 1... results1.txt)


// sleep for 1-8 random seconds
void random_sleep() {

    int sleep_time = rand() % 8 + 1;
    sleep(sleep_time);
}

// pattern 1: all processes at once
void pattern1(int num_procs) {

    // array for child PID
    pid_t pids[num_procs]; 
    srand(time(NULL)); // seeding


    for (int i = 1; i <= num_procs; i++) {
        pid_t pid = fork();

        // child process
        // if pid == child
        if (pid == 0) { 
            printf("Process %d (PID %d) beginning\n", i, getpid());

            random_sleep(); //

            printf("Process %d (PID %d) exiting\n", i, getpid());
            exit(0); 
        } 

        // parent process
        else {  
            //storign, parent waits for child
            pids[i-1] = pid; 

            printf("Main (PID %d) is creating Process %d (PID %d)\n", getpid(), i, pid);
        }
    }

    // waits for child process to complete
    for (int i = 0; i < num_procs; i++) {
        waitpid(pids[i], NULL, 0);
    }

    printf("Main process exiting\n");
}

// pattern 2: chained processes
void pattern2(int num_procs) {

    srand(time(NULL));

    for (int i = 1; i <= num_procs; i++) {

        pid_t pid = fork();

        // child process
        if (pid == 0) {  
            printf("Process %d (PID %d) beginning\n", i, getpid());

            random_sleep();

            printf("Process %d (PID %d) exiting\n", i, getpid());

            exit(0);  
        } 

        // parent process
        else {  
            printf("Process %d (PID %d) is creating Process %d\n", i-1, getppid(), i);

            waitpid(pid, NULL, 0);  
        }
    }

    printf("Main process exiting\n");
}

// pattern 3 helper: binary tree
void fork_tree(int level, int num_procs) {

    // stop if level > total number of processes
    if (level > num_procs) return;


    // create left child
    pid_t left = fork(); 

    // left child process
    // if fork() == 0: left child process
    if (left == 0) {  

        printf("Process %d (PID %d) beginning\n", level, getpid());

        random_sleep();
        
        // if left child slot is free
        if (2 * level <= num_procs) {

            printf("Process %d (PID %d) is creating Process %d\n", level, getpid(), 2 * level);
        
            // recursivly create chilren IF in limit of the number of processes
            fork_tree(2 * level, num_procs);  
        }

        printf("Process %d (PID %d) exiting\n", level, getpid());

        exit(0);
    }

    // right child
    pid_t right = fork();  
    
    // rc process
    if (right == 0) { 

        printf("Process %d (PID %d) beginning\n", level + 1, getpid());

        random_sleep();


        if (2 * (level + 1) <= num_procs) {

            printf("Process %d (PID %d) creating Process %d\n", level + 1, getpid(), 2 * (level + 1));
            

            // 
            fork_tree(2 * (level + 1), num_procs);
        }
        printf("Process %d (PID %d) exiting\n", level + 1, getpid());
        exit(0);
    }

    // wait for rc and lc to complete
    waitpid(left, NULL, 0);
    waitpid(right, NULL, 0);
}

// Pattern 3: binary tree of processes
void pattern3(int num_procs) {

    srand(time(NULL));  

    printf("Main (PID %d) creating Process 1\n", getpid());

    // start with process 1
    fork_tree(1, num_procs);  

    printf("Main process exiting\n");
}

int main(int argc, char *argv[]) {
    // sentry: args
    if (argc != 3) {
        printf("Usage is : %s <num_processes> <pattern>\n", argv[0]);
        return 1;
    }

    int num_procs = atoi(argv[1]);
    int pattern = atoi(argv[2]);

    // sentry for number of processes
    if (num_procs < 1 || num_procs > 256) {
        printf("The number of processes should be between 1 and 256.\n");
        return 1;
    }


    if (pattern == 1) {
        pattern1(num_procs);

    } 
    else if (pattern == 2) {
        pattern2(num_procs);
    } 
    else if (pattern == 3) {
        pattern3(num_procs);
    } 
    else {
        printf("Invalid Pattern: use 1, 2, or 3.\n");
        return 1;
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#define NUM_PHILOSOPHERS 2
#define SHM_SIZE NUM_PHILOSOPHERS * sizeof(int)

void take_fork(sem_t *sem) {
    sem_wait(sem);
}

void release_fork(sem_t *sem) {
    sem_post(sem);
}

int main() {
    int i;
    int status;
    pid_t pid[NUM_PHILOSOPHERS];
    sem_t *sem[NUM_PHILOSOPHERS];

    // Create shared memory
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    // Attach shared memory
    int* forks = (int*) shmat(shmid, NULL, 0);
    if (forks == (int*) -1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize shared memory
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        forks[i] = 0;
    }

    // Initialize semaphores
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        //Concate the string sem+i
        char sem_name[20];
        sprintf(sem_name, "/sem_%d", i);
        sem[i] = sem_open(sem_name, O_CREAT , (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP), 1);
        //Re-initialize the semaphore
        if(sem_init(sem[i], 1, 1) == -1){
            perror("sem_init failed");
            exit(1);
        }
        if (sem[i] == SEM_FAILED) {
            perror("sem_open failed");
            exit(1);
        }
    }

    // Create philosophers
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pid[i] = fork();

        if (pid[i] < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid[i] == 0) {
            // Code for i-th philosopher
            int left = i;
            int right = (i + 1) % NUM_PHILOSOPHERS;
            sem_t *semleft = sem[left];
            sem_t *semright = sem[right];

            printf("Philosopher %d: thinking...\n", i);
            //usleep(1000);

            printf("Philosopher %d: hungry.\n", i);
            printf("Philosopher %d: trying to take fork %d.\n", i, left);
            take_fork(semleft);
            printf("Philosopher %d: got fork %d.\n", i, left);
            //usleep(1000);


            printf("Philosopher %d: trying to take fork %d.\n", i, right);
            take_fork(semright);

            printf("Philosopher %d: got fork %d.\n", i, right);
            //usleep(1000);

            printf("Philosopher %d: eating...\n", i);
            //usleep(1000);

            release_fork(semleft);
            release_fork(semright);
            printf("Philosopher %d: released forks %d and %d.\n", i, left, right);
            exit(0);
        }
    }
}

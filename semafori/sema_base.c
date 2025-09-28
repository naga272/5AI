#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>


#ifndef CHECKER
#define CHECKER(x, msg)                                         \
    if (x) {                                                    \
        printf("%s\n%d: %s", msg, errno, strerror(errno));      \
        exit(errno);                                            \
    }
#else
#error "macro CHECKER definita altrove"
#endif


#define SIZE_BUFF 1024


void sem_op(int semid, int semnum, int op)
{
    struct sembuf sb = {
        semnum,
        op,
        0
    };
    CHECKER(semop(semid, &sb, 1) == -1, "errore in sem_op")
}


int padre(int shmid, int semid, void *shmaddr)
{
    sem_op(semid, 0, -1);

    const char *msg = "hello world\0";
    strncpy(shmaddr, msg, SIZE_BUFF);
    printf("Ho inserito nel buffer: %s", msg);

    sem_op(semid, 0, +1);

    wait(NULL);
    shmdt(shmaddr);
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}


int figlio(int shmid, int semid, void *shmaddr)
{
    sem_op(semid, 0, -1);

    printf("%s\n", (char*) shmaddr);

    sem_op(semid, 0, +1);

    shmdt(shmaddr);
    return EXIT_SUCCESS;
}


int main(int argc, char **argv, char **envp)
{
    key_t key = IPC_PRIVATE;

    int shmid = shmget(key, SIZE_BUFF, IPC_CREAT | 0666);
    CHECKER(shmid == -1, "errore in shmid")

    int semid = semget(key, 1, IPC_CREAT | 0666);
    CHECKER(semid == -1, "errore in semget")

    semctl(semid, 0, SETVAL, 1);

    pid_t pid = fork();
    CHECKER(pid == -1, "errore durante la duplicazione del processo")

    void* shm_addr = shmat(shmid, NULL, 0);

    return (pid)? padre(shmid, semid, shm_addr) : figlio(shmid, semid, shm_addr);
}


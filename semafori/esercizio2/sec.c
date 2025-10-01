#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#ifndef CHECKER
#define CHECKER(x, msg)                                             \
    if (x) {                                                        \
        printf("errore: %s\n%i: %s", msg, errno, strerror(errno));  \
        exit(errno);                                                \
    }
#endif

#define KEY 1234
#define NUM_FILOSOFI 5

static struct sembuf bf;

void sem_op(int semfd, int semnum, int op)
{
    bf.sem_num = semnum;
    bf.sem_op = op;
    bf.sem_flg = 0;

    CHECKER(
        semop(semfd, &bf, 1) == -1,
        "Errore durante la semop"
    )
}

void pensa(int filosofo_id)
{
    printf("Filosofo %d sta pensando...\n", filosofo_id);
    sleep(1);
}

void mangia(int filosofo_id)
{
    printf("Filosofo %d sta mangiando\n", filosofo_id);
    sleep(1);
    printf("Filosofo %d ha finito di mangiare\n", filosofo_id);
}

int filosofo_core(int semid, int filosofo_id)
{
    int forchetta_sx = filosofo_id;
    int forchetta_dx = (filosofo_id + 1) % NUM_FILOSOFI;
    
    for (int i = 0; i < 3; i++) {
        pensa(filosofo_id);
        
        // SOLUZIONE: Prendiamo sempre prima la forchetta con numero minore
        // per evitare deadlock
        int prima, seconda;
        if (forchetta_sx < forchetta_dx) {
            prima = forchetta_sx;
            seconda = forchetta_dx;
        } else {
            prima = forchetta_dx;
            seconda = forchetta_sx;
        }
        
        // Prendiamo la prima forchetta
        sem_op(semid, prima, -1);
        printf("Filosofo %d ha preso la forchetta %d\n", filosofo_id, prima);
        
        // Piccola pausa per ridurre la competizione
        struct timespec ts = {0, 100000000}; // 100ms
        nanosleep(&ts, NULL);
        
        // Prendiamo la seconda forchetta
        sem_op(semid, seconda, -1);
        printf("Filosofo %d ha preso la forchetta %d\n", filosofo_id, seconda);
        
        mangia(filosofo_id);
        
        // Rilasciamo le forchette (ordine inverso)
        sem_op(semid, seconda, 1);
        sem_op(semid, prima, 1);
        
        printf("Filosofo %d ha rilasciato le forchette\n", filosofo_id);
    }
    
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    int filosofi = NUM_FILOSOFI;
    key_t key = KEY;    
    pid_t pid;
    
    // Crea semafori System V
    int semid = semget(KEY, NUM_FILOSOFI, IPC_CREAT | 0666);
    CHECKER(
        semid == -1, 
        "errore durante la creazione dei semafori"
    )

    // Inizializza i semafori a 1 (forchette disponibili)
    for (int i = 0; i < NUM_FILOSOFI; i++) {
        union semun {
            int val;
            struct semid_ds *buf;
            unsigned short *array;
        } arg;
        
        arg.val = 1;
        CHECKER(
            semctl(semid, i, SETVAL, arg) == -1,
            "errore nell'inizializzazione dei semafori"
        )
    }

    printf("Inizio simulazione filosofi...\n");

    // Crea i processi filosofi
    for (int i = 0; i < filosofi; i++) {
        pid = fork();
        CHECKER(pid == -1, "errore durante la fork")

        if (pid == 0) {
            // Processo figlio = filosofo
            int result = filosofo_core(semid, i);
            exit(result);
        }
    }

    // Processo padre aspetta che tutti i figli terminino
    int status;
    for (int i = 0; i < filosofi; i++) {
        wait(&status);
    }

    printf("Tutti i filosofi hanno terminato.\n");

    // Rimuove i semafori System V
    CHECKER(
        semctl(semid, 0, IPC_RMID) == -1,
        "errore nella rimozione dei semafori"
    )
    
    return EXIT_SUCCESS;
}
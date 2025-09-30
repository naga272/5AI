
/*
* Descrizione del problema:
* 
* Cinque filosofi sono seduti a un tavolo rotondo con un piatto di spaghetti davanti a ciascuno di loro.
* Tra ogni paio di filosofi adiacenti c'è una forchetta, quindi ci sono cinque forchette in totale.
* I filosofi trascorrono la loro vita pensando e mangiando.
* Quando un filosofo ha fame, cerca di prendere le due forchette adiacenti al suo piatto.
* Se un filosofo riesce a prendere entrambe le forchette, può mangiare.
* Dopo aver mangiato, il filosofo rimette le forchette al loro posto e continua a pensare.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#ifndef CHECKER
#define CHECKER(x, msg)                                             \
    if (x) {                                                        \
        printf("errore: %s\n%i: %s", msg, errno, strerror(errno));  \
        exit(errno);                                                \
    }
#else
#error "macro CHECKER gia definita altrove, rinominala in un atrlo modo"
#endif


#define KEY 1234


/*
 * Creo una sola istanza alla struct sembuf,
 * così ogni volta che viene chiamata sem_op
 * Non deve ricreare lo spazio nello stack
*/
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


int main(int argc, char **argv, char **envp)
{
    int filosofi = 5;
    key_t key = KEY;    
    pid_t pid;
    
    /* 5 semafori che rappresenta le 5 forchette */
    int semid = semget(KEY, 5, IPC_CREAT | 0666);
    CHECKER(
        semid == -1, 
        "errore durante la creazione del semaforo"
    )

    // setto le forchette come usabili
    for (int i = 0; i < 5; i++)
        sem_op(semid, i, 1);

    // creo 5 figli (i filosofi)
    for (int i = 0; i < filosofi; i++) {
        pid = fork();
        CHECKER(pid == -1, "errore durante la fork")

        // processo padre ha il pid del figlio, il figlio ha valore 0
        if (pid)
            continue;

        if (!pid) {
            // processo figlio = filosofo
            // il filosofo prende le due forchette adiacenti a lui
            sem_op(semid, i, -1);
            sem_op(semid, (i + 1) % 5, -1); // % 5 per sicurezza in caso di seg fault

            // sta mangiando ...
            printf("sto mangiando\n");
            sleep(1);

            // il filosofo ha finito e posa le forchette
            sem_op(semid, i, +1);
            sem_op(semid, (i + 1) % 5, +1); // % 5 per sicurezza in caso di seg fault
            // il filosofo deve saltare il ciclo
            break;
        }
    }

    /* 
     * se il processo corrente e' il processo padre aspetta che 
     * finiscano tutti i processi figli.
     * Quando il processo padre non ha piu figli e chiamo la wait(),
     * questa restituisce il valore -1.
    */
    while (pid && wait(NULL) != -1);

    // printf("hello world\n");
    return EXIT_SUCCESS;
}



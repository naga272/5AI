
/*
* Descrizione del problema:
* 
* Cinque filosofi sono seduti a un tavolo rotondo con un piatto di spaghetti davanti a ciascuno di loro.
* Tra ogni paio di filosofi adiacenti c'è una forchetta, quindi ci sono cinque forchette in totale.
* I filosofi trascorrono la loro vita pensando e mangiando.
* Quando un filosofo ha fame, cerca di prendere le due forchette adiacenti al suo piatto.
* Se un filosofo riesce a prendere entrambe le forchette, può mangiare.
* Dopo aver mangiato, il filosofo rimette le forchette al loro posto e continua a pensare.
* - Le forchetto sono rappresentate dai semafori, perché sono le risorse, quindi realizzate 5 semafori (ricordatevi che un semaforo appena creato vale 0)
* - I filosofi sono rappresentati dai processi, avrete bisogno di 5 processi (eventualmente 6 se pensate a un manager o padre dei 5 processi)
* - E' bene definire (#define) usa serie di costanti per gestire al meglio la simulazione:
*      - per quanti secondi al massimo un processo dorme ovvero fa le sue filosofeggiate, un valore iniziale per fare le prove potrebbe essere 10 
*        (vi sarà una random da 1 secondo a n secondi in cui filofofeggia)
*      - quanto tempo passa da quando prende una forchetta a quando prende la seconda forchetta inizialmente questo tempo potrebbe essere 1
*      - quanto tempo ci impiega a mangiare, inizialmente questo tempo potrebbe essere 2 secondi
*      - E' consigliabile implementare gli ultimi 2 punti con 2 funzioni separate che si basano sulla define, ricordo che la funzione sleep() può essere comoda
* - Ragionare di far girare la simulazione per un certo tempo, ovvero dopo che un filosofo ha mangiato per un NUMERO costante di volte (ad esempio 10) termina, ovvero il processo muore.
*/

#include "config.h"


void sem_op(int semfd, int semnum, int op)
{
    struct sembuf bf;
    bf.sem_num = semnum;
    bf.sem_op = op;
    bf.sem_flg = SEM_UNDO;

    CHECKER(
        semop(semfd, &bf, 1) == -1,
        "<1>Errore durante la semop"
    )
}


void pensando(int time)
{
    sleep(time);
}


void mangia()
{
    printd("<2>sta mangiando\n");
    sleep(TIME_EAT);
    printd("<2>ha finito di mangiare\n");
}


int filosofo_core(int semid, int id)
{
    /*
    * Ogni filosofo esegue questa funzione:
    * @semid: fd che punta all'array di semafori
    * @id: indica il numero del filosofo che si trova davanti al filosofo
    */
    if (semid < 0)
        return EINVAL;

    for (int count_test = 0; count_test < MAX_N_TEST; count_test++) {
        /* 
        * per quanti secondi al massimo un processo
        * dorme ovvero fa le sue filosofeggiate
        */
        pensando(TEMPO_X_PENSARE);

        int psinistra = id;
        int pdestra = (id + 1) % N_FILOSOFI;

        // il filosofo deve prendere le due forchette adiacenti a lui
        if (id == N_FILOSOFI - 1) {
            sem_op(semid, psinistra, -1);
            /*
            * quanto tempo passa da quando prende una
            * forchetta a quando prende la seconda forchetta
            */
            pensando(TEMPO_X_SEC_FORK);
            sem_op(semid, pdestra, -1);
        } else {
            sem_op(semid, pdestra, -1);
            /*
            * quanto tempo passa da quando prende una
            * forchetta a quando prende la seconda forchetta
            */
            pensando(TEMPO_X_SEC_FORK);
            sem_op(semid, psinistra, -1);
        }

        printd("<4>Filosofo ha preso le forchette\n");

        mangia();

        // il filosofo ha finito e posa le forchette
        sem_op(semid, pdestra, +1);
        sem_op(semid, psinistra, +1);
        printd("<4>filosofo ha rilasciato le forchette\n");    
    }

    return EXIT_SUCCESS;
}


int main(int argc, char **argv, char **envp)
{
    key_t key = KEY;    
    pid_t pid;

    int semid = semget(KEY, N_SEMA, IPC_CREAT | 0666);
    CHECKER(
        semid == -1, 
        "<1>errore durante la creazione del semaforo"
    )

    // setto tutti i semafori a 1 (forchette disponibili)
    for (int i = 0; i < N_SEMA; i++) {
        CHECKER(
            semctl(semid, i, SETVAL, 1) == -1,
            "<1>Errore durante semctl SETVAL"
        );
    }
    /*
    * I filosofi sono rappresentati dai processi,
    * avrete bisogno di 5 processi 
    * (eventualmente 6 se pensate a un manager o padre dei 5 processi)
    */
    for (int i = 0; i < N_FILOSOFI; i++) {
        pid = fork();
        CHECKER(pid == -1, "<1>errore durante la fork")

        // processo padre ha il pid del figlio, il figlio ha valore 0
        if (pid)
            continue;

        // processo figlio = filosofo
        if (!pid)
            return filosofo_core(semid, i);
    }

    /* 
     * se il processo corrente e' il processo padre aspetta che 
     * finiscano tutti i processi figli.
     * Quando il processo padre non ha piu figli e chiamo la wait(),
     * questa restituisce il valore -1.
    */
    while (pid && wait(NULL) != -1);
    return EXIT_SUCCESS;
}

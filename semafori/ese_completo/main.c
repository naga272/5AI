
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>


#ifndef CHECKER
    #define CHECKER(x, msg)                             \
    if (x) {                                            \
        printf("%s\n", msg);                            \
        printf("%d: %s\n", errno, strerror(errno));     \
        exit(errno);                                    \
    }
#else
    #error "macro CHECKER gia' definita da qualche altra parte"
#endif

// indica il permesso read and write
#ifndef RW
    #define RW 0666
#else
    #error "macro RW gia' definita altrove"
#endif

// usato come buffer
#ifndef SHM_SIZE
    #define SHM_SIZE 1024
#endif

#ifndef SEM_EMPTY
    #define SEM_EMPTY 0
#endif

#ifndef SEM_FULL
    #define SEM_FULL 1
#endif


void sem_op(int semid, int sem_num, int op)
{
    /*
    * Uso questa procedura per tenere piu pulito la funzione
    * father_main e child_main.
    * @semid: id del set di semafori
    * @sem_num: numero di semafori in @semid
    * @op: operazioni possibili sul semaforo:
    *      -1: va a decrementare il semaforo e blocca il processo se <= 0
    *      +1: incrementa il semaforo e sblocca i processi (se sono in attesa)
    */
    struct sembuf sb = {
        sem_num,         // indice del semaforo nel set
        op,              // operazione da fare
        0                // flag da lasciare 0
    };

    /*
    * int semop(int semid, struct sembuf *sops, size_t nsops);
    * @semid: id del set
    * @sops:  array di operazioni da eseguire
    * @nsops: numero di operazioni (in questo caso 1)
    * Il kernel quando chiama questa funzione prova a decrementare
    * il semaforo semid[sb->sem_num] di 1.
    * Se il risultato e' minore di 0il processo si blocca finche un'altro processo
    * lo incrementa, altrimenti lo decrementa e continua a svolgere le sue attivita'
    * */
    CHECKER(semop(semid, &sb, 1) == -1, "error from semop");
}


int father_main(int semid, int shmid, void* shmaddr)
{
    /*  
    * CODICE CHE VIENE ESEGUITO DAL PROCESSO PADRE
    *  */

    // vedi i commenti della procedura sem_op
    sem_op(semid, SEM_EMPTY, -1);

    // dato da condividere
    const char *msg = "Ciao dal padre con System V!";

    // copio il dato nella zona da condividere col figlio 
    strncpy(shmaddr, msg, SHM_SIZE);
    // aggijungo carattere terminatore al buffer condiviso
    shmaddr[SHM_SIZE - 1] = '\0';

    // vedi i commenti della procedura sem_op
    sem_op(semid, SEM_FULL, +1);

    wait(NULL);      // aspetto la terminazione del figlio
    /*
    * int shmdt(const void *shmaddr);
    * Stacca il segmento di memoria condivisa dal processo (occhio a ptr, non e' piu valido)
    * */
    shmdt(shmaddr);
    /*
    * int semctl(int semid, int semnum, int cmd, ...);
    * Inizializza il valore di un solo semaforo
    * @semid: id del set di semafori
    * @semnum: indice del semaforo nel set
    * @cmd: comando da eseguire sul semaforo.
    *       Possono essere:
    *          - SETVAL: inizializza il valore di un semaforo singolo.
    *                    // metaforicamente: semid[SEM_EMPTY] = 1;
    *                    semctl(semid, SEM_EMPTY, SETVAL, 1);
    *          - IPC_RMID: Comunico al kernel di far nuclearizzare il set di semafori
    *                      (usala quando non ti servono piu')
    * */
    shmctl(shmid, IPC_RMID, NULL); // rimuove dalla memoria il contenuto del buffer
    semctl(semid, 0, IPC_RMID);    // rimuove semafori

    return EXIT_SUCCESS;
}


int child_main(int semid, int shmid, void* shmaddr)
{
    // vedi i commenti della procedura sem_op
    sem_op(semid, SEM_FULL, -1);
    printf("FIGLIO: letto dalla memoria -> %s\n", shmaddr);

    // vedi i commenti della procedura sem_op
    sem_op(semid, SEM_EMPTY, +1);

    /*
    * int shmdt(const void *shmaddr);
    * Stacca il segmento di memoria condivisa dal processo (occhio a ptr, non e' piu valido)
    * */
    shmdt(shmaddr);
    return EXIT_SUCCESS;
}


int main(int argc, char** argv, char** envp)
{
    /* 
    *   Questa chiave indica una risorsa condivisa tra piu' processi.
    *   Ogni segmento di memoria condivisa, 
    *   ogni set di semafori deve avere una chiave condivisa.
    *   La chiave serve al kernel per capire a quale risorsa stai facendo riferimento.
    *   Se due processi chiamano shmget o semget usando la stessa chiave, questi accedono
    *   alla medesima risorsa, se diversa accedono a una risorsa diversa oppure
    *   avviene l'inferno.
    *   il valore IPC_PRIVATE serve a dire che vogliamo una risorsa solo per processi legati dalla fork()
    *   che creeremo.
    */
    key_t key = IPC_PRIVATE;

    /*
    *   int shmget(key_t key, size_t size, int shmflg);
    *   Quello che fa in sostanza e' dire al kernel di cercare
    *   un segmento in memoria che ha @key.
    *   @key:     la chiave che identifica la risorsa
    *   @size:    dimensione del segmento in bytes (ho definito la macro SHM_SIZE = 1kb)
    *   @shmflg:  flag e permessi, citandone alcuni:
    *       - ICP_CREATE: crea il segmento se non esiste gia'
    *       - 0666:       permessi in stile UNIX (-rw-rw-rw)
    *   RET:
    *      In caso di errore restituisce -1,
    *      altrimenti restituisce un fd
    * */
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | RW);
    CHECKER(shmid == -1, "errore creazione memoria condivisa");

    /*
    * int semget(key_t key, int nsems, int semflg);
    * Chiede al kernel di trovare o creare un set di semafori con quella @key
    * @key:    solita chiave
    * @nsems:  quanti semafori vuoi nel set (nel mio caso creo un set di 2 semafori)
    * @semflg: flag | permessi 
    * RET:
    *    Se il set esiste gia', restituisce il set attuale,
    *    altrimenti crea un nuovo set di semafori con @nsems semafori.
    *    In caso di errore restituisce -1.
    *    Il valore restituito e' un valore intero che identifica il set di semafori
    * */

    int semid = semget(key, 2, IPC_CREAT | RW);
    CHECKER(semid == -1, "errore creazione semafori");

    /*
    * int semctl(int semid, int semnum, int cmd, ...);
    * Inizializza il valore di un solo semaforo
    * @semid: id del set di semafori
    * @semnum: indice del semaforo nel set
    * @cmd: comando da eseguire sul semaforo.
    *       Possono essere:
    *          - SETVAL: inizializza il valore di un semaforo singolo.
    *                    // metaforicamente: semid[SEM_EMPTY] = 1;
    *                    semctl(semid, SEM_EMPTY, SETVAL, 1);
    *          - IPC_RMID: Comunico al kernel di far nuclearizzare il set di semafori
    *                      (usala quando non ti servono piu')
    * */
    semctl(semid, SEM_EMPTY, SETVAL, 1);
    semctl(semid, SEM_FULL, SETVAL, 0);

    /*  DUPLICAZIONE DEL PROCESSO */
    pid_t pid = fork();
    CHECKER(
        pid == -1,
        "errore durante la duplicazione del processo"
    );

    /* 
    * void *shmat(int shmid, const void *shmaddr, int shmflg);
    * Il kernel mappa nello spazio di memoria il segmento
    * @shmid:   fd che indica il segmento
    * @shmaddr: indirizzo virtuale con cui vuoi mappare la memoria
    * @shmflg: flags
    *       - 0: Accesso lettura/scrittura
    *       - SHM_RDONLY: solo lettura
    * Restituisce un (void*) da usare come array per leggere / scrivere.
    * Restituisce (void*) - 1 nel caso in cui qualcosa va storto
    * */
    void* shmaddr = shmat(shmid, NULL, 0);
    CHECKER(shmaddr == (void*) -1, "errore attach memoria");

    // se il pid == 0 allora esegue il figlio, altrimenti esegue il padre
    // cosi' dealloco le risorse sconosciute della procedura _start per entrambi
    return (pid)? father_main(semid, shmid, shmaddr) : child_main(semid, shmid, shmaddr);
}

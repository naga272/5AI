#ifndef CONFIG_H
#define CONFIG_H

// https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#if __unix__
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#else
#error "puoi compilare questo codice solo con os unix-like"
#endif

#ifndef CHECKER
#define CHECKER(x, msg)                                             \
    if (x) {                                                        \
        printd(DERROR msg "\n");                                     \
        exit(errno);                                                \
    }
#else
#error "macro CHECKER gia definita altrove, rinominala in un atrlo modo"
#endif

#define DCRIT_ERROR "<0>"
#define DERROR "<1>"
#define DINFO "<2>"
#define DWARN "<3>"
#define DEBUG "<4>"


#ifndef KEY
#define KEY 9876
#else 
#error "macro KEY gia definita da qualche altra parte"
#endif

#ifndef MAX_N_TEST
#define MAX_N_TEST 10
#endif

#ifndef N_SEMA
#define N_SEMA 5
#endif

#ifndef N_FILOSOFI
#define N_FILOSOFI 5
#endif

#ifndef TEMPO_X_PENSARE
#define TEMPO_X_PENSARE 2
#endif

#ifndef TEMPO_X_SEC_FORK
#define TEMPO_X_SEC_FORK 1
#endif

#ifndef TIME_EAT
#define TIME_EAT 2
#endif


char* escapes[] = {
    "30",  // nero
    "31",  // rosso
    "32",  // verde
    "33",  // giallo
    "34",  // blu
};



// leggi printd(char* msg)
int ilog_level = 0;


void printd(char* msg)
{
    /*
    * Funzione printd che stampa stringhe a seconda del livello
    * indicato nel vettore tra <n>. Es:
    * printd("<3>" "Ciao come stai").
    * In questo caso il livello dell'informazione e' indicata con
    * valore 3, se il valore di ilog_level e' minore del valore stampato,
    * verra stampato a schermo "Ciao come stai" col colore
    * giallo.
    * Altrimenti, se il valore passato e' inferiore a ilog_level
    * significa che quell'informazione non e' importante e che quindi non bisogna
    * stamparla a schermo.
    * 
    * Possibili valori:
    *   - 0: colore nero
    *   - 1: colore rosso
    *   - 2: colore verde
    *   - 3: colore giallo
    *   - 4: colore blu
    **/
    
    if (msg[0] != '<' || msg[2] != '>') {
        printf(
            "\033[%sm"
            "errore nella formattazione del logger! %s"
            "\033[0m", 
            escapes[1], 
            msg
        );
        return;
    }

    if (msg[1] < '0' || msg[1] > '4')  {
        printf(
            "\033[%sm"
            "errore nella formattazione del logger! livello non supportato %s"
            "\033[0m", 
            escapes[1],
            msg
        );
        return;
    }

    int get_level = (int) (msg[1] - '0');
    msg += 3;               // salto "<n>"

    if (get_level >= ilog_level)
        printf(
            "\033[%sm"
            "%s"
            "\033[0m",
            escapes[get_level],
            msg
        );
}

#endif

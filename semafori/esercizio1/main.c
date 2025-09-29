#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>


int main(int argc, char **argv, char **envp)
{
    key_t key = 1234;

    int fdsem = semget(key, 1, IPC_CREAT | 0666);

    if (fdsem == -1) {
        printf("error fd sem");
        exit(EXIT_FAILURE);
    }

    struct sembuf sb = {
        0,      // semaforo numero ...
        -1,     // decrementa
        0       // flags
    };

    semop(fdsem, &sb, 1);

    return EXIT_SUCCESS;
}
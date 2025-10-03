# Problema dei 5 filosofi

![Platform](https://img.shields.io/badge/OS%20platform%20supported-Linux-green?style=flat)
![Language](https://img.shields.io/badge/Language-C-blue?style=flat) ![Language](https://img.shields.io/badge/Language-bash-green?style=flat) ![Testing](https://img.shields.io/badge/Test-Pass-green)

## Descrizione

Cinque filosofi sono seduti a un tavolo rotondo con un piatto di spaghetti davanti a ciascuno di loro. Tra ogni paio di filosofi adiacenti c'è una forchetta, quindi ci sono cinque forchette in totale. I filosofi trascorrono la loro vita pensando e mangiando. Quando un filosofo ha fame, cerca di prendere le due forchette adiacenti al suo piatto. Se un filosofo riesce a prendere entrambe le forchette, può mangiare. Dopo aver mangiato, il filosofo rimette le forchette al loro posto e continua a pensare.

#### Obiettivo

L'obiettivo è quello di sincronizzare l'accesso alle forchette in modo che i filosofi possano mangiare senza incorrere in deadlock o starvation. Un deadlock si verifica quando due o più filosofi sono bloccati in attesa l'uno dell'altro per ottenere le forchette. La starvation si verifica quando un filosofo non riesce a ottenere le forchette per un lungo periodo di tempo.

#### Soluzione usata

Utilizzo di semafori: I semafori possono essere utilizzati per gestire l'accesso alle forchette e prevenire i deadlock.

#### REALIZZAZIONE:

- Le forchetto sono rappresentate dai semafori, perché sono le risorse, quindi realizzate 5 semafori (ricordatevi che un semaforo appena creato vale 0)
- I filosofi sono rappresentati dai processi, avrete bisogno di 5 processi (eventualmente 6 se pensate a un manager o padre dei 5 processi)
- E' bene definire (#define) usa serie di costanti per gestire al meglio la simulazione:
per quanti secondi al massimo un processo dorme ovvero fa le sue filosofeggiate, un valore iniziale per fare le prove potrebbe essere 10 (vi sarà una random da 1 secondo a n secondi in cui filofofeggia)
- quanto tempo passa da quando prende una forchetta a quando prende la seconda forchetta inizialmente questo tempo potrebbe essere 1
- quanto tempo ci impiega a mangiare, inizialmente questo tempo potrebbe essere 2 secondi
- E' consigliabile implementare gli ultimi 2 punti con 2 funzioni separate che si basano sulla define, ricordo che la funzione sleep() può essere comoda
- Ragionare di far girare la simulazione per un certo tempo, ovvero dopo che un filosofo ha mangiato per un NUMERO costante di volte (ad esempio 10) termina, ovvero il processo muore.

## Requisiti

- Os linux-like
- gcc (sudo apt install gcc)

## Esecuzione

Andare nella directory princiaple dell'esercizio e eseguire il file ```build.sh```

## Author

- Federico Bastianello

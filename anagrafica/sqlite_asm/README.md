# Database sqlite3 in assembly

### Applicazione di questo Test

- Verra sicuramente testato, migliorato e reso una libreria da inserire all'interno del mio progetto [web server in nasm puro](https://github.com/naga272/server_x86_64)

## Requisiti

- assembler nasm
- linker ld
- devi aver installato il pacchetto libsqlite3-dev.

```sh
#!/bin/bash
sudo apt install nasm binutils libsqlite3-dev
```

## Esecuzione programma

### Warning: Script bash provato solo su macchina Ubuntu

- Avviare il file bash ```build.sh```
- Eseguire l'eseguibile ```main``` generato da ```build.sh``` 

## Descrizione

Programma che crea un database chiamato ```anagrafica.sqlite3``` (se inesistente, altrimenti ci accede) e crea la tabella Persone e Citta:

```sql
CREATE TABLE Citta(
    cod_id int not null,
    Istat varchar(8) not null,
    Comune varchar(256) not null,
    Provincia varchar(256) not null,
    Regione varchar(256) not null,
    Prefisso varchar(256) not null,
    CAP varchar(256) not null,
    CodFisco varchar(256) UNIQUE not null,
    Abitanti varchar(256) not null,
    Link varchar(256) not null,

    PRIMARY KEY(cod_id)
);


CREATE TABLE Persone(
    id_pers integer NOT NULL,
    id_provincia integer NOT NULL,
    nome varchar(32) NOT NULL,
    cognome varchar(32) NOT NULL,
    codFisc varchar(16) NOT NULL,
    PRIMARY KEY(id_pers, id_provincia),
    foreign key(id_provincia) REFERENCES Citta(cod_id)
);
```

Una volta create con successo queste due tabelle, inserisce una tupla all'interno della tabella Citta

```sql
INSERT INTO Citta ([cod_id], [Istat], [Comune], [Provincia], [Regione], [Prefisso], [CAP], [CodFisco], [Abitanti], [Link])
VALUES (
    0, "28001", "Abano Terme", "PD",
    "VEN", "49", "35031",
    "A001", "19726", "http://www.comuni-italiani.it/028/001/"
);
```

### Documentazione

Le funzioni di questo modulo assembly si basano sulla libreria installata tramite apt libsqlite3-dev.

Questo modulo contiene le seguenti funzioni:

- ```int sqlite3_open("<nome_file>.db", &db_handle)```: Crea o apre un file sqlite, restituisce un valore != 0 in caso di errore
- ```void do_table_sqlite(db_handle*, char*)```: funzione usata per creare una tabella all'interno del db sqlite.
- ```void do_insert_sqlite(db_handle*, char*)```: funzione usata per inserire una tuapla all'interno della tabella
- ```sqlite3_close(db_handle)```: chiude l'istanza al db

Ecco un esempio di uso di queste funzioni:

Supponiamo di avere creato in .data le seguenti variabili:

```asm
dbfile db "anagrafica.sqlite3", 0x00
```
Che va a rappresentare il nomde del database.

Ora ci dobbiamo occupare di creare le due tabelle:
```asm
citta_table:
    db "CREATE TABLE Citta("
    db      "cod_id int not null,"
    db      "Istat varchar(8) not null,"
    db      "Comune varchar(256) not null,"
    db      "Provincia varchar(256) not null,"
    db      "Regione varchar(256) not null,"
    db      "Prefisso varchar(256) not null,"
    db      "CAP varchar(256) not null,"
    db      "CodFisco varchar(256) UNIQUE not null,"
    db      "Abitanti varchar(256) not null,"
    db      "Link varchar(256) not null,"
    db      "PRIMARY KEY(cod_id)"
    db ");", 0x00


persone_table:
    db "CREATE TABLE Persone("
    db      "id_pers integer NOT NULL,"
    db      "id_provincia integer NOT NULL,"
    db      "nome varchar(32) NOT NULL,"
    db      "cognome varchar(32) NOT NULL,"
    db      "codFisc varchar(16) NOT NULL,"
    db      "PRIMARY KEY(id_pers, id_provincia),"
    db      "foreign key(id_provincia) REFERENCES Citta(cod_id)"
    db ");", 0x00
```

E infine impostiamo una query per inserire una tupla all'interno della tabella Citta

```asm
; ora ci sono i problemi con la traduzione dei metacaratteri, non si puo usare \".
; in questi casi uso il metodo "DO KISS", lo rappresento direttamente con il char ASCII (0x22)
sql_insert_citta:  
    db "INSERT INTO Citta ([cod_id], [Istat], [Comune], [Provincia], [Regione], [Prefisso], [CAP], [CodFisco], [Abitanti], [Link])"
    db "VALUES ("
    db      "0, ", 0x22, "28001", 0x22, ", ", 0x22, "Abano Terme", 0x22, ", ", 0x22, "PD", 0x22, ","
    db      0x22, "VEN", 0x22, ", ", 0x22, "49", 0x22, ", ", 0x22, "35031", 0x22, ","
    db      0x22, "A001", 0x22, ", ", 0x22, "19726", 0x22, ", ", 0x22, "http://www.comuni-italiani.it/028/001/", 0x22, ""
    db ");", 0x00
```

E in section bss abbiamo:

```asm
section .bss
    db_handle   resq 1
```

Che rappresenta l'istanza del database aperto.

Nella section text dobbiamo prima accedere al db, creare le tabelle, inserire poi i dati e infine chiudere l'istanza.

```asm
    ; sqlite3_open("<nome_file>.db", &db_handle)
    ; sqlite3_open se il db non viene trovato, lo crea
    mov rdi, dbfile
    lea rsi, [rel db_handle]
    call sqlite3_open  ; rax != 0 errore

    ; creazione della table citta
    mov rdi, [db_handle]
    mov rsi, citta_table
    call do_table_sqlite

    ; creazione table persone
    mov rdi, [db_handle]
    mov rsi, persone_table
    call do_table_sqlite

    ; insert di una tupla nella tabella citta
    mov rdi, [db_handle]
    mov rsi, sql_insert_citta
    call do_insert_sqlite

    ; sqlite3_close(db_handle)
    mov rdi, [db_handle]
    call sqlite3_close
```


Se anche tu vuoi usare sqlite.asm come un modulo e non come programma main, vai all'interno del codice di sqlite.asm e modifica la riga dove è scritto:
```asm
; togli questa macro se vuoi usare questo file come modulo
%define TESTING_SQLITE
```

in:

```asm
; togli questa macro se vuoi usare questo file come modulo
; %define TESTING_SQLITE
```

Ora puoi importare questa lib nel tuo file assembly molto facilmente in questo modo:

```asm
%include "./<path>/sqlite3.asm"
```

## Author

- naga272
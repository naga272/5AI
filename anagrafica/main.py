import pandas as pd
import sys
import os


response = '''
CREATE TABLE Citta(
    cod_id int              not null,
    Istat varchar(8)        not null,
    Comune varchar(256)     not null,
    Provincia varchar(256)  not null,
    Regione varchar(256)    not null,
    Prefisso varchar(256)   not null,
    CAP varchar(256)        not null,
    CodFisco varchar(256)   UNIQUE not null,
    Abitanti varchar(256)   not null,
    Link varchar(256)       not null,

    PRIMARY KEY(cod_id)
);


CREATE TABLE Persone(
    id_pers integer         NOT NULL,
    id_provincia integer    NOT NULL,
    nome varchar(16)        NOT NULL,
    cognome varchar(16)     NOT NULL,
    codFisc varchar(16)     NOT NULL,
    NumTelefono varchar(16) NOT NULL,
    PRIMARY KEY(id_pers, id_provincia),
    foreign key(id_provincia) REFERENCES Citta(cod_id)
);

'''


def main(argc: int, argv: list) -> int:
    global response

    if argc < 3:
        print(
            "error! devi passare gli argomenti della funzione!\n"
            "$python3 main.py <inp_comuni.txt> <name_script.sql>"
        )
        return 1

    if not os.path.exists(sys.argv[1]):
        print(
            "error! file non trovato, assicurati di eseguire"
            f"il file input correttamente {sys.argv[1]}"
        )
        return 1

    df = pd.read_csv(sys.argv[1], sep=';', encoding="UTF-8")

    print("sto elaborando i dati...")
    # @nome_colonne non si basano sull'intestazione del csv
    # ma sul nome assegnato alle colonne del database
    # il csv potrebbe aver malformata l'intestazione e
    # se eseguo le query crasha tutto
    nome_colonne = (
        "[cod_id], [Istat], [Comune], "
        "[Provincia], [Regione], [Prefisso], [CAP], "
        "[CodFisco], [Abitanti], [Link]"
    )

    for cod_id, tupla in df.iterrows():
        response += (
            f"INSERT INTO Citta ({nome_colonne})\n"
            "VALUES (\n"
            f"    {cod_id}, \"{tupla.Istat}\", \"{tupla.Comune}\", \"{tupla.Provincia}\",\n"
            f"    \"{tupla.Regione}\", \"{tupla.Prefisso}\", \"{tupla.CAP}\",\n"
            f"    \"{tupla.CodFisco}\", \"{tupla.Abitanti}\", \"{tupla.Link}\"\n"
            ");\n\n"
        )

    # faccio una insert per Persone
    response += (
        "INSERT INTO Persone(id_pers, id_provincia, nome, cognome, codFisc)\n"
        "VALUES(0, 2, \"Lorem\", \"IPSUM\", \"LOREMIPSUMM23\");"
    )

    with open("myfile.sql", "w", encoding="utf-8") as f_sql:
        f_sql.write(response)

    return 0


if __name__ == "__main__":
    result = main(len(sys.argv), sys.argv)
    print(f"programma terminato con valore: {result}")
    sys.exit(result)

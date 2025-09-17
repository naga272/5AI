# 5AI

## ⚠️ Warning ⚠️

Per nessuna ragione deve venir toccato questo branch, ognuno lavora al proprio.

Se non hai ben presente come si fa ti spiego in questo tutorial


## Tutorial Base

### Requisiti:

- Account [Github](https://github.com)
- Programma [Git](https://git-scm.com/downloads)


### Copiarsi in locale il progetto remoto

Apri bash e incolla questo comando

```sh
git clone https://github.com/naga272/5AI.git
```

In questo modo crei una cartella all'interno della directory in cui ti trovi in quel momento.

### Creazione di un nuovo Branch in locale con git

```sh
git switch -c tuo_nick_github
```
Il comando ```switch``` dice di spostarti in quel branch e, se si aggiunge il flag ```-c```, dici che se non esiste prima crealo e poi spostami la.

**In caso hai sbagliato a assegnare il nome del tuo branch**, prima esci da quel branch:
```sh
git switch main
```

e poi digiti il comando:

```sh
git branch -D nome_branch
```

Se hai già caricato il branch col nome sbagliato online, digita il seguente comando per eliminarlo anche da la:

```sh
git push origin --delete nome_branch
```

### Push delle modifiche al tuo branch su github

```sh
git switch tuo_nick_github    # per sicurezza
git add .
git commit -m "Some Text"
git push -u origin tuo_nick_github
```

Col comando ```add``` aggiungi file o da modificare che devono essere pushati.
Col comando ```commit``` è una conferma delle modifiche, aggiungo il flag ```-m ""``` se vuoi aggiungere rapidamente una descrizione delle modifiche, altrimenti se ti serve scrivere tanta roba non aggiungerlo.

# Carica da remoto a locale

Il comando ```pull``` si usa per quando la mia macchina client ha il branch non aggiornato e devo aggiornarlo con la versione del branch che si trova su github

Se hai fatto il push con:
```sh
git push -u origin tuo_nick_github
```

Allora per aggiornare il branch basta scrivere

```sh
git pull
```

Perche sa gia' come e' tracciato il tuo branch locale.

**Se hai solo creato il branch in locale ma non lo hai collegato:**
```sh
git pull origin tuo_nick_github
```


## author

- naga272

# Domotic System

## Introduzione
`Domotic System` è un programma progettato per gestire dispositivi in un ambiente domotico. Offre un'**interfaccia testuale semplice** per controllare dispositivi, 
visualizzarne lo stato e configurarne le impostazioni.

## Funzionalità Principali
- ### Controllo dei Dispositivi
  Gli utenti possono accendere, spegnere e programmare i dispositivi in base alle loro esigenze. È possibile impostare orari specifici per l'accensione e lo spegnimento dei dispositivi e vederne i consumi in tempo reale.

- ### Monitoraggio dello Stato
  Il sistema permette di visualizzare lo stato attuale di tutti i dispositivi, inclusi quelli accesi, spenti e programmati. Gli utenti possono ottenere informazioni dettagliate sul consumo energetico e sulla produzione di energia di ciascun dispositivo.

- ### Registrazione delle Operazioni
  Ad ogni esecuzione del programma, viene creato un file di log che registra tutte le operazioni effettuate. Questo file di log viene salvato all'interno della cartella DomoticSystemLogs, con un nome standard dato da data e ora di avvio del programma (esempio: 10-01-25 18_45_17-Log.txt). Gli utenti hanno anche la possibilità di specificare un nome personalizzato per il file di log all'avvio del programma.

## Aspetti tecnici di base
Ogni `dispositivo` contiene un `id` univoco, differente da qualunque altro dispositivo, e un `numero di serie` specifico per il suo tipo.

Per gestire i dispositivi disponibili, è stato creato il file `ListaDispositivi.h`, che contiene un vettore con tutti i dispositivi utilizzabili. Questo approccio consente di aggiungere, rimuovere o modificare i dispositivi in qualsiasi momento senza dover modificare direttamente sul codice del programma.

La classe `RicercaDispositivo` implementa una funzionalità di ricerca fuzzy basilare, che permette di individuare un dispositivo nella lista anche se il nome fornito è impreciso, correggendo errori di battitura o nomi parziali.

Questa funzionalità è utilizzata dalla classe `CreaDispositivo`, che consente di creare un dispositivo specifico fornendo un nome (anche non esatto), l’orario di inizio e, opzionalmente, l’orario di fine. La classe recupera le caratteristiche del dispositivo dalla `ListaDispositivi.h` e lo configura in modo automatico.

La gestione dei dispositivi e' stata realizzata tramite Lista. 
La classe LinkedList implementa una gestione avanzata dei dispositivi tramite una doubly linked list, sfruttando smart pointers per garantire una corretta gestione della memoria. Questa struttura è stata progettata per operazioni efficienti di inserimento, rimozione e ricerca, adattandosi alle necessità di un sistema domotico che richiede flessibilità e affidabilità.
Ogni oggetto LinkedList è composto da:
- HEAD e TAIL: Shared pointers ai nodi in testa e coda della lista.
- STATUS: Stringa che indica lo stato dei dispositivi nella lista (acceso/spento/programmato).

Ogni nodo contiene:
- Un std::unique_ptr a un oggetto Dispositivo.
- Shared pointers ai nodi successivo (NEXT) e precedente (PREV).

La classe LinkedList.h e' stata estesa per gestire nuove politiche relative ai tipi di dispositivi della lista corrispondente grazie alla sua progettazione modulare e l'uso di ereditarietà, nel caso corrente:
- LinkedListOn.h    (FIFO)
- LinkedListOff.h   (LIFO)
- LinkedListProg.h  (FIFO)

Infine, la classe `Interfaccia` riunisce tutti i componenti del progetto, integrando i dispositivi, i contenitori e le funzionalità di gestione in un unico sistema domotico.

## Comandi supportati dal programma

### Il programma supporta i seguenti comandi:

- `set ${DEVICENAME} on`: Accende il dispositivo (sia CP che M)
- `set ${DEVICENAME} off`: Spegne il dispositivo
- `set ${DEVICENAME} ${START} [${STOP}]`: Imposta avvio programmato
- `rm ${DEVICENAME}`: Rimuove il timer associato al dispositivo
- `show`: Mostra tutti i dispositivi attivi e inattivi
- `show ${DEVICENAME}`: Mostra a schermo produzione/consumo energetico di uno specifico dispositivo reset time: Reimposta l'orario corrente
- `set time ${TIME}`: Va a una specifica ora del giorno
- `reset time`: **Comando per il debug**. Resetta il tempo del sistema, riportandolo all’orario 00:00. Riporta tutti i dispositivi alle condizioni iniziali
- `reset timers`: **Comando per il debug**. Rimuove i timer di tutti i dispositivi. Tutti i dispositivi rimangono nel loro stato attuale (accesi o spenti)
- `reset all`: **Comando per il debug**. Riporta il sistema alle condizioni iniziali. L’orario viene impostato a 00:00, tutti i timer vengono rimossi. Tutti i dispositivi vengono spenti
- `show debug`: **Comando per il debug**. Si tratta di uno show particolare che mostra tutte le liste e i loro dispositivi

## Istruzioni per la Compilazione

### Per compilare con cmake:
Viene utilizzata `ottimizzazione O2`


### Per compilare senza cmake:

    c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\Interfaccia.cpp src\LinkedList.cpp src\LinkedListOff.cpp src\LinkedListOn.cpp src\LinkedListProg.cpp src\RicercaDispositivo.cpp src\mainInterfaccia.cpp -o mainInterface

### Per eseguire il programma con un file di istruzioni:

    .\mainInterface.exe | Get-Content istruzioni.txt 

### Per utilizzare un file di istruzioni:
1) Creare un file chiamato istruzioni.txt
2) Inserire fromFile come prima riga
3) Specificare y per creare un file di log con nome specifico seguito dal nome del file di log, oppure n per lasciare nome dibase
4) Scrivere ogni istruzione su una riga separata
5) Terminare il file con il comando q

#### Esempio file di istruzioni
    fromFile
    y
    logfile.txt
    set televisore on on
    set time 12:30
    show 
    q

## Membri del gruppo

* Alberto Bortoletto - 2101761
* Fabrizio Scabbia - 2111533
* Michele Stevanin - 2101741

## Struttura del Progetto
La quantità di codice è stata distribuita il più **equamente** possibile tra i membri. Il numero di file scritti da ciascun membro non rappresenta la quantità di lavoro svolto. Vale la pena notare che il file `interfaccia.cpp` contiene una quantità di codice maggiore rispetto a qualsiasi altro file.
  ```plaintext
    DOMOTIC-SYSTEM/
    ├── include/
    │   ├── CreaDispositivo.h                       # Michele Stevanin
    │   ├── Dispositivo.h                           # Michele Stevanin
    │   ├── Interfaccia.h                           # Fabrizio Scabbia
    │   ├── LinkedList.h                            # Alberto Bortoletto
    │   ├── LinkedListOff.h                         # Alberto Bortoletto
    │   ├── ListaDispositivi.h                      # Michele Stevanin
    │   └── RicercaDispositivo.h                    # Michele Stevanin
    ├── src/
    │   ├── CreaDispositivo.cpp                     # Michele Stevanin
    │   ├── Dispositivo.cpp                         # Michele Stevanin
    │   ├── Interfaccia.cpp                         # Fabrizio Scabbia
    │   ├── LinkedListOff.cpp                       # Alberto Bortoletto
    │   ├── mainInterfaccia.cpp                     # Alberto Bortoletto
    │   └── RicercaDispositivo.cpp                  # Michele Stevanin
    │
    ├── README.md                                   # Documentazione generale del progetto
    ├── CMakeLists.txt                              # CMakeLists
    ├── .gitignore                                  # gitignore
    └── ProgettoFinale-SistemaDomotico - v2.pdf     # Consegna del progetto
  ```
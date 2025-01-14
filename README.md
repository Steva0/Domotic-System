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

La classe `RicercaDispositivo` implementa una funzionalità di ricerca fuzzy basilare, che permette di individuare un dispositivo nella lista anche se il nome fornito è impreciso, correggendo errori di battitura o nomi parziali (ad esempio, "telvisore" o "telvsore" invece di "televisore").

Questa funzionalità è utilizzata dalla classe `CreaDispositivo`, che consente di creare un dispositivo specifico fornendo un nome (anche non esatto), l’orario di inizio e, opzionalmente, l’orario di fine. La classe recupera le caratteristiche del dispositivo dalla `ListaDispositivi.h` e lo configura in modo automatico.

La gestione dei dispositivi e' stata realizzata tramite **lista**. 
La classe `LinkedList` implementa una gestione avanzata dei dispositivi tramite una doubly linked list, sfruttando smart pointers per garantire una corretta gestione della memoria.

Ogni oggetto `LinkedList` è composto da:
- HEAD e TAIL: Shared pointers ai nodi in testa e coda della lista.
- STATUS: Stringa che indica lo stato dei dispositivi nella lista (acceso/spento/programmato).

Ogni nodo contiene:
- Un **std::unique_ptr** a un oggetto Dispositivo.
- Shared pointers ai nodi successivo (**NEXT**) e precedente (**PREV**).

La classe LinkedList.h e' stata estesa per gestire nuove politiche relative ai tipi di dispositivi della lista corrispondente:
- `LinkedListOn`    (FIFO)
- `LinkedListOff`   (LIFO)
- `LinkedListProg`  (FIFO)

La classe `Interfaccia` gestisce il sistema domotico, sfruttando le funzionalità della classe `Dispositivo` e delle classi `LinkedList` per dispositivi accesi, spenti e programmati. 

Consente il controllo di dispositivi con monitoraggio energetico e registrazione log, garantendo che il consumo totale non superi mai i kilowatt disponibili, spegnendo i primi dispositivi accesi se necessario.

Riconosce automaticamente i comandi utente, lanciando errori specifici (`WrongCommandSyntax` e `InvalidTimeFormat`) in caso di sintassi o formati temporali errati, per una migliore comunicazione con l'utente. 

Per garantire maggiore flessibilità e precisione, sono state implementate diverse funzioni che consentono di gestire più dispositivi dello stesso tipo. Per differenziarli, è sufficiente indicare il nome del dispositivo insieme al numero di serie (nel formato `NomeDispositivo-NumeroSerie`, con valore predefinito `-1` se non specificato).

Sono inoltre gestiti tutti i possibili conflitti relativi a timer e/o dispositivi programmati. In tali casi, l'utente ha la possibilità di scegliere come procedere, basandosi sulle opzioni fornite dal programma.

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
### Comandi extra
- `show devices`: Mostra tutta la lista di possibili dispositivi e le loro caratteristiche
- `help`: **Comando di aiuto**. Mostra la lista dei comandi disponibili con le giuste sintassi
- `show debug`: **Comando per il debug**. Si tratta di uno show particolare che mostra tutte le liste e i loro dispositivi

Per ogni comando inserito durante l'esecuzione, il programma verifica la validità della sintassi e, se necessario, anche la correttezza degli orari specificati. In caso di errore di sintassi, viene mostrata a schermo la sintassi corretta; in caso di errore nella formattazione dell'orario, viene indicato il formato corretto.

## Gestione dei dipositivi
In caso di sovraccarico di potenza, è stata implementata una politica di tipo `First In, First Out`, con l'eccezione dei dispositivi che hanno il flag `isSempreAcceso` impostato a `true`. Questi dispositivi, come il frigorifero, non vengono spenti, ma si passa direttamente al dispositivo successivo.

I dispositivi non sono hardcoded nel codice, ma gestiti tramite un vettore separato, consentendo di aggiungere, rimuovere o modificarne le caratteristiche senza dover modificare la classe principale o ricompilare il codice. Per dimostrare questa flessibilità, è stato aggiunto un numero significativo di dispositivi con caratteristiche diverse.

### Dispositivi Extra
| Dispositivo Manuale        | Potenza (kW) | Dispositivo a Ciclo Prefissato | Potenza (kW) | Durata Ciclo (minuti) |  
|----------------------------|--------------|--------------------------------|--------------|-----------------------|  
| Computer                   | -0.8         | Condizionatore                 | -1.8         | 300                   |  
| Server                     | -2.0         | Proiettore                     | -1.0         | 120                   |  
| Aspirapolvere              | -0.9         | Stereofono                     | -0.3         | 30                    |  
| Forno                      | -1.6         | Stampante                      | -0.6         | 4                     |  
| Console Wii                | -0.4         | Friggitrice ad aria            | -1.2         | 100                   |  
| Piano a induzione          | -1.5         | Deumidificatore                | -1.0         | 50                    |  
| Allarme                    | -0.4         | Tostapane                      | -0.3         | 5                     |  
| Citofono                   | -0.2         | Piastra per capelli            | -0.1         | 10                    |  
| Stufa elettrica            | -2.5         | Amplificatore                  | -0.8         | 50                    |  
| Lampada LED                | -0.05        | Macchina per il pane           | -0.8         | 150                   |  
| Ventilatore                | -0.6         | Scanner                        | -0.4         | 5                     |  
| Tagliaerba elettrico       | -1.5         |                                |              |                       |  
| Climatizzatore portatile   | -1.2         |                                |              |                       |  
| Lampada da tavolo          | -0.05        |                                |              |                       |  
| Congelatore                | -0.6         |                                |              |                       |  
| Macchina per il gelato     | -0.4         |                                |              |                       |  
| Frigorifero smart          | -0.5         |                                |              |                       |  
| Frullatore                 | -0.2         |                                |              |                       |  
| Centrale nucleare          | +4000000     |                                |              |                       |  

## Istruzioni per la Compilazione

Durante la compilazione tramite `cmake` viene utilizzata `ottimizzazione O2`

### Per compilare senza cmake:

    g++ src/CreaDispositivo.cpp src/Dispositivo.cpp src/Interfaccia.cpp src/LinkedList.cpp src/LinkedListOff.cpp src/LinkedListOn.cpp src/LinkedListProg.cpp src/RicercaDispositivo.cpp src/mainInterfaccia.cpp -o DomoticSystem

### Per eseguire il programma con un file di istruzioni:

#### Comando per Windows
    Get-Content istruzioni.txt | ./DomoticSystem.exe
#### Comando per Gnu/Linux
    ./DomoticSystem < istruzioni.txt

### Per utilizzare un file di istruzioni:
1) Creare un file chiamato istruzioni.txt
2) Specificare `y` per scegliere il nome del file di log e inserirlo alla riga successiva, controllando che esso sia valido; oppure `n` per lasciare decidere al programma un nome di base
3) Inserire `fromFile` per vedere sul terminale anche i comandi letti dal file
4) Scrivere ogni istruzione su una riga separata tenendo delle possibili interazioni necessarie
5) Terminare il file con il comando `q`

#### Esempio file di istruzioni
    y
    logfile.txt
    fromFile
    set televisore on
    set time 12:30
    show 
    q

## Membri del gruppo

    Alberto Bortoletto  - 2101761
    Fabrizio Scabbia    - 2111533
    Michele Stevanin    - 2101741

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
    │   ├── LinkedListOn.h                          # Alberto Bortoletto
    │   ├── LinkedListProg.h                        # Alberto Bortoletto
    │   ├── ListaDispositivi.h                      # Michele Stevanin
    │   └── RicercaDispositivo.h                    # Michele Stevanin
    ├── src/
    │   ├── CreaDispositivo.cpp                     # Michele Stevanin
    │   ├── Dispositivo.cpp                         # Michele Stevanin
    │   ├── Interfaccia.cpp                         # Fabrizio Scabbia
    │   ├── LinkedList.cpp                          # Alberto Bortoletto
    │   ├── LinkedListOff.cpp                       # Alberto Bortoletto
    │   ├── LinkedListOn.cpp                        # Alberto Bortoletto
    │   ├── LinkedListProg.cpp                      # Alberto Bortoletto
    │   ├── mainInterfaccia.cpp                     # Fabrizio Scabbia
    │   └── RicercaDispositivo.cpp                  # Michele Stevanin
    ├── Esempi-Log/
    │   ├── istruzioni-1.txt                        # Esempio istruzioni
    │   ├── istruzioni-2.txt                        # Esempio istruzioni
    │   ├── logFile-1.txt                           # Log esempio istruzioni-1
    │   └── logFile-1.txt                           # Log esempio istruzioni-2
    │
    ├── README.md                                   # Documentazione generale del progetto
    ├── CMakeLists.txt                              # CMakeLists
    └── ProgettoFinale-SistemaDomotico - v2.pdf     # Consegna del progetto
  ```

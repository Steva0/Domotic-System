# Domotic System

## Introduzione
`Domotic System` è un programma progettato per gestire dispositivi in un ambiente domotico. Offre un'**interfaccia testuale semplice** per controllare dispositivi, visualizzarne lo stato e registrare le operazioni.
Ad ogni esecuzione viene salvato un file di log all'interno della cartella DomoticSystemLogs.

## Aspetti tecnici di base
Ogni `dispositivo` contiene un `id` univoco, differente da qualunque altro dispositivo, e un `numero di serie` specifico per il suo tipo.

Per gestire i dispositivi disponibili, è stato creato il file `ListaDispositivi.h`, che contiene un vettore con tutti i dispositivi utilizzabili. Questo approccio consente di aggiungere, rimuovere o modificare i dispositivi in qualsiasi momento senza dover modificare direttamente sul codice del programma.

La classe `RicercaDispositivo` implementa una funzionalità di ricerca fuzzy basilare, che permette di individuare un dispositivo nella lista anche se il nome fornito è impreciso, correggendo errori di battitura o nomi parziali.

Questa funzionalità è utilizzata dalla classe `CreaDispositivo`, che consente di creare un dispositivo specifico fornendo un nome (anche non esatto), l’orario di inizio e, opzionalmente, l’orario di fine. La classe recupera le caratteristiche del dispositivo dalla `ListaDispositivi.h` e lo configura in modo automatico.

Per contenere i dispositivi è stata progettata una classe `LinkedList`, che utilizza shared_ptr per garantire una gestione sicura ed efficiente della memoria. Inoltre, è stata creata una classe derivata, `LinkedListOff`, per gestire in maniera ottimale i dispositivi spenti.

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

## Istruzioni per la Compilazione

### Per compilare con cmake:
Viene utilizzata **ottimizzazione O2**

    mkdir build
    cd build
    cmake ..
        
### Per compilare senza cmake:

    c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\RicercaDispositivo.cpp src\LinkedList.cpp src\LinkedListOff.cpp src\Interfaccia.cpp src\mainInterfaccia.cpp -o mainInterface

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

* Alberto Bortoletto
* Fabrizio Scabbia
* Michele Stevanin

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
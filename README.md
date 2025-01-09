# Domotic System

## Introduzione
`Domotic System` è un programma progettato per gestire dispositivi in un ambiente domotico. Offre un'**interfaccia testuale semplice** per controllare dispositivi, visualizzarne lo stato e registrare le operazioni.
Ad ogni esecuzione viene salvato un file di log all'interno della cartella DomoticSystemLogs.

## Membri del gruppo

* Alberto Bortoletto
* Fabrizio Scabbia
* Michele Stevanin

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
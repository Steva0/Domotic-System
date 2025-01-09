
`set time` [Funziona]

`set $device on/off` [Funziona]

`set $device start stop` [Funziona]

`rm $device`  [Funziona]

`show`  [Funziona]

`show $device`  [Funziona]

`reset time`  [Funziona]

`reset timers` [Funziona]

`reset all` [Funziona]


## Suddivisione Domotic System

Per compilare il main usare questa linea di comando
  ```
  c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\RicercaDispositivo.cpp src\LinkedList.cpp src\LinkedListOff.cpp src\Interfaccia.cpp src\mainInterfaccia.cpp -o mainInterface
  ```

Per compilare usando un file txt di istuzioni usare questa linea di comando
  ```
  c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\RicercaDispositivo.cpp src\LinkedList.cpp src\LinkedListOff.cpp src\Interfaccia.cpp src\mainInterfaccia.cpp -o mainInterface && Get-Content tester.txt | .\mainInterface.exe
  ```
Istruzioni:
1) chiamare il file tester.txt e scrivere ogni istruzione in una riga
2) alla prima riga inserire la keyword "fromFile"
3) Se si vuole specificare un nome del file di log 
    - Scivere y e nella riga dopo specificare il nome del file di log
    - Altrimenti scrivere n
4) Scrivere tutti i comandi
5) Terminare il file con la q


**Michele:**
 - classe dispositivi
 - readme e CMake

**Alberto**:
 - gestione dei vari dispositivi (linked list)
 - main

**Fabrizio**:
 - interfaccia utente

**Tutti assieme appassionatamente:**

 - documentazione


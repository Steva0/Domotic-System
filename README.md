
`set time` [PARZIALE] [Ultimi ritocchi finali]

`set $device on/off` [Funziona]

`set $device start stop` [Funziona]

`rm $device`  [PARZIALE] [Non funziona con CP] [Forse va tolto anche orario di accensione]

`show`  [WIP]

`show $device`  [WIP]

`reset time`  [WIP]

`reset timers` [PARZIALE] [Non funziona con CP] [Forse va tolto anche orario di accensione]

`reset all` [WIP]


## Suddivisione Domotic System

Per compilare il main usare questa linea di comando
  ```
  c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\RicercaDispositivo.cpp src\main.cpp -o programma
  ```
Per compilare il mainNode usare questa linea di comando
  ```
  c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\RicercaDispositivo.cpp src\LinkedList.cpp src\mainNode.cpp -o programma
  ```
Per compilare il mainInterface usare questa linea di comando
  ```
  c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\RicercaDispositivo.cpp src\LinkedList.cpp src\LinkedListOff.cpp src\Interfaccia.cpp src\mainInterfaccia.cpp -o mainInterface
  ```
**Michele:**

 - classe dispositivi
 - readme e CMake

*circa 30%*

**Alberto**:

 - gestione dei vari dispositivi (linked list)

*circa 30%*

**Fabrizio**:

 - interfaccia utente

*circa 30%*

**Tutti assieme appassionatamente:**

 - documentazione

*circa 10%*


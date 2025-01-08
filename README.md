
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


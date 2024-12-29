

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

 - classe dispositivi (+ id specifico tipo 00-01-02-03...)

*circa 30%*

**Alberto**:
 - gestione dei vari dispositivi (linked list, array)

*circa 30%*

**Fabrizio**:
interfaccia utente: 

 - vari comandi utente e main (gestione orari)

*circa 30%*

**Tutti assieme appassionatamente:**

 - readme e CMake

 - documentazione

*circa 10%*


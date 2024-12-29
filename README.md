

## Suddivisione Domotic System

Per compilare il main usare questa linea di comando
  ```
  c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\RicercaDispositivo.cpp src\main.cpp -o programma
  ```
Per compilare il mainNode usare questa linea di comando
 ```
  c++ src\CreaDispositivo.cpp src\Dispositivo.cpp src\RicercaDispositivo.cpp src\LinkedList.cpp src\mainNode.cpp -o programma
  ```
**Michele:**

 - classe dispositivi virtuale (+ id specifico tipo 00-01-02-03...)
 - classe dispositivi manuale virtuale
 - classe dispositivi automatica virtuale
 - classi concrete dei vari dispositivi (manuali e automatici) (+id di
   serie del dispositivo, tipo televisore1, televisore2 ect..)

*circa 30%*

**Alberto**:
 - gestione dei vari dispositivi (linked list)

*circa 30%*

**Fabrizio**:
interfaccia utente: 

 - sistema di log,
 - sistema di stampe,
 - vari comandi utente e main (gestione orari)

*circa 30%*

**Tutti assieme appassionatamente:**

 - readme e CMake

 - documentazione

*circa 10%*


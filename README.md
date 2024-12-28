

## Suddivisione Domotic System

per compilare usare questa linea di comando
  ```
  c++ src\code\CreaDispositivo.cpp src\code\Dispositivo.cpp src\code\RicercaDispositivo.cpp src\code\main.cpp -o programma
  ```


**Michele:**

 - classe dispositivi virtuale (+ id specifico tipo 00-01-02-03...)
 - classe dispositivi manuale virtuale
 - classe dispositivi automatica virtuale
 - classi concrete dei vari dispositivi (manuali e automatici) (+id di
   serie del dispositivo, tipo televisore1, televisore2 ect..)

*circa 30%*

**Alberto**:
interfaccia utente:
 - gestione dei vari dispositivi (linked list),

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


//Alberto Bortoletto
#ifndef LINKEDLISTON_H
#define LINKEDLISTON_H

#include "LinkedList.h"
#include "Dispositivo.h"

class LinkedListOn: public LinkedList
{
    public:

    //Costruttori

    LinkedListOn();                                                                     //costruttore di default
    LinkedListOn(Dispositivo& dispositivo);                                             //costruttore con inserimento di un dispositivo in coda

    void incrementTimeOn();                                                             //incrementa il tempo di tutti i dispositivi accesi                  
    double getConsumoAttuale() const;                                                   //restituisce il consumo totale        
    double producedEnergy() const;                                                      //restituisce l'energia prodotta in questo minuto
    double consumedEnergy() const;                                                      //restituisce il modulo dell'energia consumata in questo minuto               
    std::vector<Dispositivo> removeAllDevicesOff(const int currentTime);                //rimuove tutti i dispositivi spenti (il cui orario di spegnimento e' prima dell'orario indicato) e restituisce tutti i dispositivi spenti in un vector            
};

#endif // LINKEDLISTON_H
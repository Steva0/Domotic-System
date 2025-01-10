//Alberto Bortoletto
#ifndef LINKEDLISTON_H
#define LINKEDLISTON_H

#include "LinkedList.h"
#include "Dispositivo.h"

class LinkedListOn: public LinkedList
{
    public:
    LinkedListOn();
    LinkedListOn(Dispositivo& dispositivo);

    void incrementTimeOn();                                                             //incrementa il tempo di tutti i dispositivi accesi                  
    double getConsumoAttuale() const;                                                   //restituisce il consumo totale        
    double getProdotta() const;                                                         //restituisce il modulo dell'energia consumata    
    double getUsata() const;                                                            //restituisce l'energia prodotta                            
};

#endif // LINKEDLISTON_H
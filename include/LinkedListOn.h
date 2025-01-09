#ifndef LINKEDLISTON_H
#define LINKEDLISTON_H

#include "LinkedList.h"
#include <vector>
#include "Dispositivo.h"

class LinkedListOn: public LinkedList
{
    public:
    void incrementTimeOn();                                                             //incrementa il tempo di tutti i dispositivi accesi        -  LINKED LIST ON            
    double getConsumoAttuale(int currentTime) const;                                    //restituisce il consumo totale                            - LINKED LIST ON
    double getProdotta() const;                                                         //restituisce il modulo dell'energia consumata            - LINKED LIST ON
    double getUsata() const;                                                            //restituisce l'energia prodotta                            - LINKED LIST ON
};

#endif // LINKEDLISTON_H
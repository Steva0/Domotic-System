//Alberto Bortoletto
#ifndef LINKEDLISTPROG_H
#define LINKEDLISTPROG_H

#include "LinkedList.h"
#include <vector>
#include "Dispositivo.h"

class LinkedListProg: public LinkedList
{
    public:
    LinkedListProg();
    LinkedListProg(Dispositivo& dispositivo);
    std::vector<Dispositivo> getDevicesToPowerOn(const int currentTime);                                 //Ritorna tutti i dispositivi che vengono accesi all'orario corrente 
    std::string showAll(int currentTime) const;                                                          //mostra il consumo di ogni nodo 
};

#endif // LINKEDLISTPROG_H
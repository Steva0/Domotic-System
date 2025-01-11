//Alberto Bortoletto
#ifndef LINKEDLISTPROG_H
#define LINKEDLISTPROG_H

#include "LinkedList.h"
#include "Dispositivo.h"
#include <vector>

class LinkedListProg: public LinkedList
{
    public:

    //-------Costruttori-------
    LinkedListProg();
    LinkedListProg(Dispositivo& dispositivo);

    //-------Funzioni membro-------
    std::vector<Dispositivo> removeDevicesToPowerOn(const int currentTime);                 //Rimuove e ritorna tutti i dispositivi che vengono accesi all'orario corrente - Puo' lanciare un'eccezione std::out_of_range("Lista vuota!")
    std::string showAll(int currentTime) const;                                             //mostra il consumo di ogni nodo 

    //-------Distruttori-------
    
    ~LinkedListProg() = default;                                                             //distruttore default in quanto la classe gestisce risorse dinamiche tramite smart pointers
};

#endif // LINKEDLISTPROG_H
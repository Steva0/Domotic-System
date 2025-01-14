//Alberto Bortoletto
#ifndef LINKEDLISTOFF_H
#define LINKEDLISTOFF_H

#include "LinkedList.h"
#include "Dispositivo.h"
class LinkedListOff: public LinkedList
{   
    public:

    //-------Costruttori-------

    LinkedListOff();                                                     //costruttore di default
    LinkedListOff(Dispositivo& dispositivo);                            //costruttore con inserimento di un dispositivo in coda

    //-------Funzioni membro-------

    void insert(Dispositivo& dispositivo) override;                     //inserisce un dispositivo in modo corretto nella lista secondo l'orario di accensione (Vedi LinkedList.cpp)

    //-------Distruttori-------
    
    ~LinkedListOff() = default;                                         //distruttore default in quanto la classe gestisce risorse dinamiche tramite smart pointers
};

#endif // LINKEDLISTOFF_H
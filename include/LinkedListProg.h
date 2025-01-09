#ifndef LINKEDLISTON_H
#define LINKEDLISTON_H

#include "LinkedList.h"
#include <vector>
#include "Dispositivo.h"

class LinkedListProg: public LinkedList
{
    public:
    std::vector<Dispositivo> turnOnDevices(const int currentTime);                      //Ritorna tutti i dispositivi che vengono accesi all'orario corrente    - LINKED LIST PROG

};

#endif // LINKEDLISTON_H
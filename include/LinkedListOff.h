#ifndef LINKEDLISTOFF_H
#define LINKEDLISTOFF_H

#include "LinkedList.h"
#include <vector>
#include "Dispositivo.h"

class LinkedListOff: public LinkedList
{
    public:
    void insert(Dispositivo& dispositivo) override;
};

#endif // LINKEDLISTOFF_H
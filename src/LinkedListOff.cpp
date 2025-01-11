//Alberto Bortoletto
#include "../include/LinkedListOff.h"

//Costruttore di default che chiama LinkedList() e modifica la stringa status
LinkedListOff::LinkedListOff(): LinkedList()
{ 
    status = "[SPENTO]";
}

//Costruttore con inserimento di un dispositivo in coda che chiama LinkedList(dispositivo) e modifica la stringa status
LinkedListOff::LinkedListOff(Dispositivo& dispositivo): LinkedList(dispositivo)
{ 
    status = "[SPENTO]";
}

//Inserisce un dispositivo nella lista in modo ordinato in base all'orario di accensione secondo una politica LIFO
void LinkedListOff::insert(Dispositivo& dispositivo)
{
    std::shared_ptr<Node> newNode = std::make_shared<Node>(dispositivo);
    if(isEmpty())
    {
        head = tail = newNode;
        return;
    }
    else
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}
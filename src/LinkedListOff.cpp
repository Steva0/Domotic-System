//Alberto Bortoletto
#include "../include/LinkedListOff.h"

LinkedListOff::LinkedListOff(): LinkedList(), status{"SPENTO"}
{ }

LinkedListOff::LinkedListOff(Dispositivo& dispositivo): LinkedList(dispositivo), status{"SPENTO"}
{ }

void LinkedListOff::insert(Dispositivo& dispositivo)
{
    std::shared_ptr<Node> newNode = std::make_shared<Node>(dispositivo);
    if(isEmpty())
    {
        head = tail = newNode;
        return;
    }
    else   //significa che lo aggiungo alla fine di tutti, quindi dopo tail
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}
#include "../include/LinkedListOff.h"

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
        tail->next = newNode.get();
        newNode->prev = tail.get();
        tail = newNode;
    }
}
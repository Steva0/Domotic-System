#include "../include/LinkedListOff.h"

void LinkedListOff::insert(Dispositivo& dispositivo)
{
    Node* newNode = new Node(dispositivo);
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
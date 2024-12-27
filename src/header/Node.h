//Alberto Bortoletto
#ifndef NODE_H
#define NODE_H
#include "CreaDispositivo.h"

class Node
{
    friend class LinkedList;
    private:
    Dispositivo* disp;
    Node* prev;
    Node* next;

    //costruttore
    Node(Dispositivo& data);

    //copy constructor
    Node(const Node& data);

    //copy assignment
    Node& operator=(const Node& data);
};

#endif
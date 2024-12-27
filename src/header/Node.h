//Alberto Bortoletto
#ifndef NODE_H
#define NODE_H
#include "CreaDispositivo.h"

class Node
{
    private:
    Dispositivo* disp;
    Node* prev;
    Node* next;

    public:
    //costruttore
    Node(Dispositivo& data);

    //copy constructor
    Node(const Node& data);

    //copy assignment
    Node& operator=(const Node& data);

    //getter
    Dispositivo& getDispositivo() const;
    Node& getNext() const;
    Node& getPrev() const;

};

#endif
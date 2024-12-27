#ifndef LINKED_LIST_H
#define LINKE_LIST_H
#include "Node.h"

class LinkedList
{
    private:
    Node* head;

    public:
    //Costruttore
    LinkedList(Node& firstNode);

    //Funzioni membro utili 
    void append(Node& nodeToAppend);
    void remove();
    void appendAfter(Node& afterNode);

    //Distruttiri
    ~LinkedList();


};

#endif
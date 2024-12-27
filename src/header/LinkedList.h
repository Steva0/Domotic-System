#ifndef LINKED_LIST_H
#define LINKE_LIST_H
#include "Node.h"

class LinkedList
{
    private:
    Node* head;
    Node* tail;

    public:
    //Costruttore
    LinkedList();

    //Funzioni membro utili
    void insert(Dispositivo& dispositivo);
    void remove();
    bool isEmpty();
    void setTail();
    void connectNodes(Node* before, Node* after);

    //Distruttiri
    ~LinkedList();
};

#endif
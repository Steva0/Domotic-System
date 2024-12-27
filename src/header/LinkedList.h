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
    bool isEmpty() const;
    void connectNodes(Node* before, Node* after);

    //getter
    const Node* getHead() const;  //Pericoloso perche' mi restituisce head, ma ha senso?

    //Distruttiri
    ~LinkedList();
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);

#endif
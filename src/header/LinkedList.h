#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "CreaDispositivo.h"
#include <ostream>

class LinkedList
{
    private:
    class Node
    {
        public:
        Dispositivo* disp;
        Node* prev;
        Node* next;
        Node(Dispositivo& data);

        //copy constructor
        Node(const Node& data);

        //copy assignment
        Node& operator=(const Node& data);
    };
    
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
    std::string toString() const;

    //Distruttiri
    ~LinkedList();
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);

#endif
#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "CreaDispositivo.h"
#include <iostream>
#include <ostream>

class LinkedList
{
    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list);
    
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
    LinkedList(Dispositivo& dispositivo);

    //Funzioni membro utili
    void insert(Dispositivo& dispositivo);
    void removeDispositivo(std::string nome);
    bool isEmpty() const;
    void connectBefore(Node* before, Node* after);
    void removeTimer(std::string nome);

    //Distruttiri
    ~LinkedList();
};

//overloading operator<<


#endif
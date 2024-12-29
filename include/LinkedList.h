#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "CreaDispositivo.h"
#include "DispositivoNonTrovatoException.h"
#include "ListaVuotaException.h"
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

        ~Node();
    };
    
    Node* head;
    Node* tail;

    //Funzioni private
    Node* searchDispositivo(const std::string nome) const;                  //cerca un dispositivo nella lista
    void connectBefore(Node* p, Node* q);                                   //connette prima di p il nodo q

    public:
    //Costruttore
    LinkedList();
    LinkedList(Dispositivo& dispositivo);

    //Funzioni membro utili
    void insert(Dispositivo& dispositivo);                                  //inserisce un dispositivo in coda
    Dispositivo* removeDispositivo(const std::string nome);                         //rimuove un dispositivo dalla lista accettando il nome del dispositivo
    std::vector<Dispositivo*> removeAllDispositiviOff(const int currentTime);     //rimuove tutti i dispositivi spenti (la cui ora e' prima dell'orario indicato)
    void removeTimer(const std::string nome);                               //rimuove il timer di un dispositivo
    void removeAllTimers();                                                 //rimuove tutti i timer
    bool isEmpty() const;                                                   //controlla se la lista è vuota
    double show(std::string nome) const;                                    //mostra il consumo di un dispositivo
    double showAll() const;                                                 //mostra il consumo totale della lista

    //Distruttiri
    ~LinkedList(); //Ciao
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);

#endif // LINKED_LIST_H
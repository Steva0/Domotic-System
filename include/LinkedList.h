#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "CreaDispositivo.h"
#include <iostream>
#include <ostream>
#include <exception>

class LinkedList
{
    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list);
    
    protected:
    class Node
    {
        public:
        Dispositivo* disp;
        Node* prev;
        Node* next;
        
        Node(Dispositivo& data);

        ~Node();
    };
    
    Node* head;
    Node* tail;

    //Funzioni private
    Node* searchDispositivoName(const std::string nome) const;                          //cerca un dispositivo nella lista by NAME
    Node* searchDispositivoId(const int id) const;                                      //cerca un dispositivo nella lista by ID
    void connectBefore(Node* p, Node* q);                                               //connette prima di p il nodo q

    public:
    //Costruttore
    LinkedList();
    LinkedList(Dispositivo& dispositivo);

    //Funzioni membro utili
    virtual void insert(Dispositivo& dispositivo);                                      //inserisce un dispositivo in coda
    Dispositivo* removeDispositivoName(const std::string nome);                         //rimuove un dispositivo dalla lista accettando il nome del dispositivo by NAME
    Dispositivo* removeDispositivoId(const int id);                                     //rimuove un dispositivo dalla lista accettando il nome del dispositivo by ID
    std::vector<Dispositivo*> removeAllDispositiviOff(const int currentTime);           //rimuove tutti i dispositivi spenti (la cui ora e' prima dell'orario indicato)
    
    Dispositivo* forceRemoveFirst();                                                    //rimuove il primo dispositivo 
    Dispositivo* removeFirst();                                                         //rimuove il primo dispositivo che non è sempre acceso (isSempreAcceso())
    double getConsumoAttuale(int currentTime) const;                                    //restituisce il consumo totale
    
    void removeTimer(const std::string nome);                                           //rimuove il timer di un dispositivo
    void removeAllTimers();                                                             //rimuove tutti i timer
    bool contains(const std::string nome) const;
    bool isEmpty() const;                                                               //controlla se la lista è vuota
    double show(std::string nome) const;                                                //mostra il consumo di un dispositivo
    std::string showAll() const;                                                        //mostra il consumo di ogni nodo
    
    //Distruttiri
    ~LinkedList();
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);

#endif // LINKED_LIST_H

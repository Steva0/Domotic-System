#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "CreaDispositivo.h"
#include <iostream>
#include <ostream>
#include <exception>
#include <memory>

class LinkedList
{
    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list);
    
    protected:
    class Node
    {
        public:
        std::unique_ptr<Dispositivo> disp;
        std::shared_ptr<Node> next;
        std::shared_ptr<Node> prev;
        
        Node(const Dispositivo& data);

        ~Node() = default;
    };
    
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;

    //Funzioni private
    std::shared_ptr<Node> searchDispositivoName(const std::string nome) const;                          //cerca un dispositivo nella lista by NAME
    std::shared_ptr<Node> searchDispositivoId(const int id) const;                                      //cerca un dispositivo nella lista by ID
    void checkEmpty() const;                                                           //controlla se la lista è vuota

    public:
    //Costruttore
    LinkedList();
    LinkedList(Dispositivo& dispositivo);

    //Funzioni membro utili
    virtual void insert(Dispositivo& dispositivo);                                      //inserisce un dispositivo in coda
    Dispositivo removeDispositivoName(const std::string& nome);                         //rimuove un dispositivo dalla lista accettando il nome del dispositivo by NAME
    Dispositivo removeDispositivoId(const int id);                                     //rimuove un dispositivo dalla lista accettando il nome del dispositivo by ID
    std::vector<Dispositivo> removeAllDispositiviOff(const int currentTime);           //rimuove tutti i dispositivi spenti (la cui ora e' prima dell'orario indicato)
    Dispositivo forceRemoveFirst();                                                    //rimuove il primo dispositivo 
    Dispositivo removeFirst();                                                         //rimuove il primo dispositivo che non è sempre acceso (isSempreAcceso())
    
    //turnOnDevices(const int currentTime);                                              accende i dispositivi
    //resetDevice(const std::string nome);                                               resetta un dispositivo
    //resetAll(); 
    double getConsumoAttuale(int currentTime) const;                                    //restituisce il consumo totale
    void removeTimer(const std::string nome);                                           //rimuove il timer di un dispositivo
    void removeAllTimers();                                                             //rimuove tutti i timer
    bool contains(const std::string nome) const;
    bool isEmpty() const;                                                               //controlla se la lista è vuota
    double show(std::string nome) const;                                                //mostra il consumo di un dispositivo
    std::string showAll() const;                                                        //mostra il consumo di ogni nodo
    
    //Distruttiri
    ~LinkedList() = default;
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);

#endif // LINKED_LIST_H

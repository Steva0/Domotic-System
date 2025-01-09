//Alberto Bortoletto 
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "CreaDispositivo.h"
#include <iostream>
#include <ostream>
#include <exception>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cmath>

class LinkedList
{
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
    std::string status;

    //Funzioni private
    std::shared_ptr<Node> searchDispositivoName(const std::string nome) const;                          //cerca un dispositivo nella lista by NAME
    std::shared_ptr<Node> searchDispositivoId(const int id) const;                                      //cerca un dispositivo nella lista by ID
    void checkEmpty() const;                                                                            //controlla se la lista è vuota
    Dispositivo removeNode(std::shared_ptr<Node> current);                                              //rimuove un nodo dalla lista

    public:
    //Costruttore
    LinkedList();
    LinkedList(Dispositivo& dispositivo);

    //Funzioni membro utili
    virtual void insert(Dispositivo& dispositivo);                                      //inserisce un dispositivo in coda
    Dispositivo removeDispositivoName(const std::string& nome);                         //rimuove un dispositivo dalla lista accettando il nome del dispositivo by NAME
    Dispositivo removeDispositivoId(const int id);                                      //rimuove un dispositivo dalla lista accettando il nome del dispositivo by ID 
    std::vector<Dispositivo> removeAllDispositiviOff(const int currentTime);            //rimuove tutti i dispositivi spenti (la cui ora e' prima dell'orario indicato) 
    std::vector<Dispositivo> removeAllForce();                                          //rimuove tutti i dispositivi   
    Dispositivo forceRemoveFirst();                                                     //rimuove il primo dispositivo  
    Dispositivo removeFirst();                                                          //rimuove il primo dispositivo che non è sempre acceso (isSempreAcceso()) 
            
    void removeTimer(const std::string nome, const int currentTime);                    //rimuove il timer di un dispositivo                        
    void resetAllTimers(int currentTime);                                               //rimuove tutti i timer                                     
    void resetAll();                                                                    //resetta tutti i dispositivi                               
    bool contains(const std::string nome) const;                                        //controlla se un dispositivo è presente nella lista        
    bool isEmpty() const;                                                               //controlla se la lista è vuota                             
    double show(std::string nome) const;                                                //mostra il consumo di un dispositivo                       
    std::string inlinePrint() const;                                                    //mostra la lista in linea                                  
    std::string showAll() const;                                                        //mostra il consumo di ogni nodo                            
    std::string showAllDebug() const;                                                   //mostra il consumo di ogni nodo con debug                                                                         
    
    //Distruttori
    ~LinkedList() = default;
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);                     //overloading dell'operatore << per la LinkedList (Specificare nel readme!)

#endif // LINKEDLIST_H

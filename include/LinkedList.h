//Alberto Bortoletto 
#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "CreaDispositivo.h"
#include <iostream>
#include <ostream>
#include <exception>
#include <memory>
#include <sstream>
#include <iomanip>

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
    virtual void insert(Dispositivo& dispositivo);                                      //inserisce un dispositivo in coda - COMUNE
    Dispositivo removeDispositivoName(const std::string& nome);                         //rimuove un dispositivo dalla lista accettando il nome del dispositivo by NAME - COMUNE
    Dispositivo removeDispositivoId(const int id);                                      //rimuove un dispositivo dalla lista accettando il nome del dispositivo by ID - COMUNE
    std::vector<Dispositivo> removeAllDispositiviOff(const int currentTime);            //rimuove tutti i dispositivi spenti (la cui ora e' prima dell'orario indicato) - LINKED LIST ON
    std::vector<Dispositivo> removeAllForce();                                          //rimuove tutti i dispositivi   - COMUNI
    Dispositivo forceRemoveFirst();                                                     //rimuove il primo dispositivo  - COMUNE
    Dispositivo removeFirst();                                                          //rimuove il primo dispositivo che non è sempre acceso (isSempreAcceso()) - COMUNE
    
    std::vector<Dispositivo> turnOnDevices(const int currentTime);                      //Ritorna tutti i dispositivi che vengono accesi all'orario corrente    - LINKED LIST PROG
        
    void removeTimer(const std::string nome, const int currentTime);                    //rimuove il timer di un dispositivo                        - COMUNE
    void resetAllTimers(int currentTime);                                               //rimuove tutti i timer                                     - COMUNE
    void resetAll();                                                                    //resetta tutti i dispositivi                               - COMUNE
    bool contains(const std::string nome) const;                                        //controlla se un dispositivo è presente nella lista        - COMUNE
    bool isEmpty() const;                                                               //controlla se la lista è vuota                             - COMUNE
    double show(std::string nome) const;                                                //mostra il consumo di un dispositivo                       - COMUNE 
    std::string inlinePrint() const;                                                    //mostra la lista in linea                                  - COMUNE
    std::string showAll() const;                                                        //mostra il consumo di ogni nodo                            - COMUNE
    std::string showAllDebug() const;                                                   //mostra il consumo di ogni nodo con debug                  - COMUNE                                                       
    
    //Distruttori
    ~LinkedList() = default;
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);                     //overloading dell'operatore << per la LinkedList (Specificare nel readme!)

#endif // LINKED_LIST_H

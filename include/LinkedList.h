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

    std::shared_ptr<Node> searchDispositivoName(const std::string nome) const;                          //cerca un dispositivo nella lista in base al nome di quel dispositivo
    std::shared_ptr<Node> searchDispositivoId(const int id) const;                                      //cerca un dispositivo nella lista in base all'id di quel dispositivo
    Dispositivo removeNode(std::shared_ptr<Node> current);                                              //rimuove un nodo dalla lista
    void checkEmpty() const;                                                                            //controlla se la lista è vuota - la funzione può lanciare un'eccezione std::out_of_range("Lista vuota!")

    public:

    //Costruttori

    LinkedList();                                                                       //costruttore di default
    LinkedList(Dispositivo& dispositivo);                                               //costruttore con inserimento di un dispositivo in coda

    //Funzioni membro

    virtual void insert(Dispositivo& dispositivo);                                      //inserisce un dispositivo in coda in modo corretto
    Dispositivo removeDispositivoName(const std::string& nome);                         //rimuove un dispositivo dalla lista accettando il nome del dispositivo in base al nome di quel dispositivo
    Dispositivo removeDispositivoId(const int id);                                      //rimuove un dispositivo dalla lista accettando il nome del dispositivo in base all'id di quel dispositivo
    Dispositivo forceRemoveFirst();                                                     //rimuove il primo dispositivo  
    Dispositivo removeFirst();                                                          //rimuove il primo dispositivo che non è sempre acceso
    std::vector<Dispositivo> removeAll();                                               //rimuove tutti i dispositivi nella lista e restituisce tutti i dispositivi in un vector 
            
    void removeTimer(const std::string nome, const int currentTime);                    //rimuove il timer di un dispositivo - la funzione può lanciare un'eccezione std::invalid_argument("Orario di accensione non valido.");                       
    void removeAllTimers(int currentTime);                                              //rimuove tutti i timer di tutti i dispositivi - la funzione può lanciare un'eccezione std::invalid_argument("Orario di accensione non valido.");                                  
    void resetAll();                                                                    //ripristina l'orario di accensione e di spegnimento di tutti i dispositivi                               
    bool contains(const std::string nome) const;                                        //controlla se un dispositivo è presente nella lista        
    bool isEmpty() const;                                                               //controlla se la lista è vuota                             
    double show(std::string nome) const;                                                //mostra il consumo di un dispositivo                       
    std::string inlinePrint() const;                                                    //mostra la lista in linea                                  
    std::string showAll() const;                                                        //mostra il consumo di ogni dispositivo nella lista                            
    std::string showAllDebug() const;                                                   //mostra il consumo di ogni dispositivo nella lista                                                                         
    
    //Distruttori
    ~LinkedList() = default;
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);                     //overloading dell'operatore << per la LinkedList (Specificare nel readme!)

#endif // LINKEDLIST_H

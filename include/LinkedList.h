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
    class Node                                                                          //classe annidata Node
    {
        public:
        std::unique_ptr<Dispositivo> disp;                                              //smart pointer al dispositivo
        std::shared_ptr<Node> next;                                                     //smart pointer al prossimo nodo
        std::shared_ptr<Node> prev;                                                     //smart pointer al nodo precedente
        
        Node(const Dispositivo& data);                                                  //costruttore con parametro

        ~Node() = default;
    };
    
    //-------Variabili d'istanza-------
    
    std::shared_ptr<Node> head;                                                        //smart pointer alla testa della lista
    std::shared_ptr<Node> tail;                                                        //smart pointer alla coda della lista   
    std::string status;                                                                //indica lo stato dei dispositivi nella lista

    //-------Funzioni private-------

    std::shared_ptr<Node> searchDispositivo(const std::string nome) const;             //cerca un dispositivo nella lista in base al nome di quel dispositivo
    std::shared_ptr<Node> searchDispositivo(const int id) const;                       //cerca un dispositivo nella lista in base all'id di quel dispositivo
    Dispositivo removeNode(std::shared_ptr<Node> current);                             //rimuove un nodo dalla lista
    void checkEmpty() const;                                                           //controlla se la lista è vuota - la funzione può lanciare un'eccezione std::out_of_range("Lista vuota!")

    public:

    //-------Costruttori-------

    LinkedList();                                                                       //costruttore di default
    LinkedList(Dispositivo& dispositivo);                                               //costruttore con inserimento di un dispositivo in coda

    //-------Funzioni membro-------

    //Funzioni di inserimento
    virtual void insert(Dispositivo& dispositivo);                                      //inserisce un dispositivo in modo corretto nella lista secondo l'orario di accensione (Vedi LinkedList.cpp)

    //Funzioni di rimozione del dispositivo
    Dispositivo removeDispositivo(const std::string& nome);                             //rimuove e restituisce un dispositivo dalla lista accettando il nome del dispositivo in base al nome di quel dispositivo - Puo' lanciare un'eccezione throw std::out_of_range("Lista vuota!");
    Dispositivo removeDispositivo(const int id);                                        //rimuove e restituisce un dispositivo dalla lista accettando il nome del dispositivo in base all'id di quel dispositivo - Puo' lanciare un'eccezione throw std::out_of_range("Lista vuota!");
    Dispositivo forceRemoveFirst();                                                     //rimuove e restituisce il primo dispositivo non generatore
    Dispositivo removeFirst();                                                          //rimuove e restituisce il primo dispositivo che non è sempre acceso - Puo' lanciare un'eccezione throw std::out_of_range("Lista vuota!");
    std::vector<Dispositivo> removeAll();                                               //rimuove tutti i dispositivi nella lista e restituisce tutti i dispositivi in un vector 
            
    //Funzioni di rimozione dei timer
    void removeTimer(const std::string nome, const int currentTime);                    //rimuove il timer di un dispositivo                  
    void removeAllTimers(int currentTime);                                              //rimuove tutti i timer di tutti i dispositivi                                 
    void resetAll();                                                                    //ripristina l'orario di accensione e di spegnimento di tutti i dispositivi

    //Funzioni di controllo                              
    bool contains(const std::string nome) const;                                        //controlla se un dispositivo è presente nella lista
    bool contains(const int id) const;                                                  //controlla se un dispositivo è presente nella lista
    bool isEmpty() const;                                                               //controlla se la lista è vuota

    //Funzioni di visualizzazione                             
    double show(std::string nome) const;                                                //ritorna il consumo di un dispositivo con quel nome                   
    std::string showDevicesNames() const;                                               //ritorna i nomi dei dispositivi nella lista in una stringa                                
    std::string showAll() const;                                                        //mostra il consumo di ogni dispositivo nella lista                            
    std::string showAllDebug() const;                                                   //mostra il consumo di ogni dispositivo nella lista in modalità debug                                                                         
    
    //-------Distruttore-------
    
    ~LinkedList() = default;                                                            //distruttore default in quanto la classe gestisce risorse dinamiche tramite smart pointers
};

//overloading operator<<
std::ostream& operator<<(std::ostream& os, const LinkedList& list);                     //overloading dell'operatore << per la stampa della lista

#endif // LINKEDLIST_H

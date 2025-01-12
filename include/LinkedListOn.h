//Alberto Bortoletto
#ifndef LINKEDLISTON_H
#define LINKEDLISTON_H

#include "LinkedList.h"
#include "Dispositivo.h"
class LinkedListOn: public LinkedList
{
    public:

    //-------Costruttori-------

    LinkedListOn();                                                                     //costruttore di default
    LinkedListOn(Dispositivo& dispositivo);                                             //costruttore con inserimento di un dispositivo in coda

    //-------Funzioni membro-------

    std::vector<Dispositivo> removeDevicesToPowerOff(const int currentTime);                //rimuove tutti i dispositivi spenti (il cui orario di spegnimento e' prima dell'orario indicato) e li restituisce tutti in un vector            

    void incrementTimeOn();                                                             //incrementa il tempo di tutti i dispositivi accesi in un minuto - Può lanciare eccezione std::invalid_argument e std::out_of_range                
    double getCurrentConsumption() const;                                               //restituisce il consumo totale        
    double producedEnergy() const;                                                      //restituisce l'energia prodotta in questo minuto
    double consumedEnergy() const;                                                      //restituisce il modulo dell'energia consumata in questo minuto

    //-------Distruttori-------
    
    ~LinkedListOn() = default;                                                           //distruttore default in quanto la classe gestisce risorse dinamiche tramite smart pointers  
};

#endif // LINKEDLISTON_H
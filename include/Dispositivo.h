//Michele Stevanin 
#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <string>
#include <ostream>
#include <stdexcept>
#include "ListaDispositivi.h"

class Dispositivo {

    friend bool operator==(const Dispositivo&, const Dispositivo&);
    friend std::ostream& operator<<(std::ostream&, const Dispositivo&);

private:
    static std::unordered_map<std::string, int> numeroSerieDispositivi;
    static int lastId;

protected:
    std::string nome;   // Nome del tipo di dispositivo
    int id;           // Id univoco per ogni dispositivo
    int numeroSerie;  // Numero di serie specifico per il tipo di dispositivo
    double potenza;         // Positivo per produzione, negativo per consumo
    bool sempreAcceso;      // Indica se il dispositivo non può mai essere spento
    int orarioAccensione;   // In minuti da 0 (00:00) a 1439 (23:59)
    int orarioSpegnimento;  // In minuti da 0 (00:00) a 1439 (23:59)
    int durataCiclo;        //se manuale == 0 se CP != 0
    int tempoAccensione;    //tempo tot accensione
    bool timerOn;         //true se timer attivo

public:
    // Costruttore
    Dispositivo();
    Dispositivo(const std::string& nome, double potenza, int durataCiclo, bool sempreAcceso = false, int orarioAccensione = 0, int orarioSpegnimento = 0, bool hasTimer = false);
    ~Dispositivo() = default;
    // Operatore di copia
    Dispositivo& operator=(const Dispositivo& other);


    //funzioni generali

    double calcolaConsumoEnergetico() const;        //calcola i kW consumati in totale in kWh
    std::string showAllInfo() const;                //nome + numeroSerie, id,  potenza, sempreAcceso, orarioAccensione, orarioSpegnimento, tempoAccensione
    std::string showInfo() const;                   //nome + numeroSerie, id, tempoAccensione
    std::string showSmall() const;                  //nome + numeroSerie, id
    std::string showConsumo() const;                //nome + numeroSerie, consumo energetico
    std::string showName() const;                   //nome + numeroSerie


    //getter

    std::string getTipo() const;                    //nome del tipo di dispositivo
    std::string getNome() const;                    //nome + numeroSerie
    int getId() const;                              //id
    int getNumeroSerie() const;                     //numeroSerie
    double getPotenza() const;                      //potenza
    bool isSempreAcceso() const;                    //sempreAcceso (true - false)
    int getOrarioAccensione() const;                //orarioAccensione
    int getOrarioSpegnimento() const;               //orarioSpegnimento
    int getTempoAccensione() const;                 //tempoAccensione
    int getDurataCiclo() const;                     //durataCiclo
    bool isManual() const;                          //true se durataCiclo == 0, false se durataCiclo != 0
    bool isCP() const;                              //true se durataCiclo != 0, false se durataCiclo == 0
    void setTimerOff();                             //rimuove timer di spegnimento, solo ai manuali
    bool isAcceso(int currentTime) const;           //true se acceso, false se spento
    bool isGenerator() const;                       //true se generatore, false se consumatore
    bool hasTimer() const;                          //restituisce true se il dispositivo ha un timer attivo


    //setter
    
    void setOrarioAccensione(int minuti, bool isTimer = false);           //setta orarioAccensione                 
    void setOrarioSpegnimento(int minuti, bool isTimer = false);          //setta orarioSpegnimento
    void incrementaTempoAccensione(int minuti);                           //incrementa tempoAccensione                 
    void resetTempoAccensione();                                          //resetta tempoAccensione a 0
    void setHasTimer(bool hasTimer);                                      //setta boolTimer

    //variabili statiche

    static const int MINUTI_GIORNATA = 1440;
    static const int MAX_MINUTI_GIORNATA = 1439;
};

#endif // DISPOSITIVO_H

std::string trasformaOrario(int minuti);                

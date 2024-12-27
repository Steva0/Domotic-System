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
    static int lastId;

protected:
    std::string nome;
    const int id; //Id univoco per ogni dispositivo
    const int numeroSerie; // Numero di serie specifico per il tipo di dispositivo
    double potenza; // Positivo per produzione, negativo per consumo
    bool sempreAcceso; // Indica se il dispositivo non può mai essere spento
    int orarioAccensione; // In minuti da 0 a 1439
    int orarioSpegnimento; // In minuti da 0 a 1439
    int durataCiclo; //se manuale = 0 se CP != 0
    int tempoAccensione; //tempo tot accensione

public:
    //costruttori
    Dispositivo(const std::string& nome, double potenza, int durataCiclo, bool sempreAcceso = false, int orarioAccensione = 0, int orarioSpegnimento = 0); //può lanciare eccezione std::invalid_argument
    virtual ~Dispositivo() = default;

    //metodi generali
    double calcolaConsumoEnergetico() const;
    std::string showAllInfo() const;            //nome + numeroSerie, id,  potenza, sempreAcceso, orarioAccensione, orarioSpegnimento, tempoAccensione
    std::string showInfo() const;               //nome + numeroSerie, id, tempoAccensione
    std::string showSmall() const;              //nome + numeroSerie, id
    std::string showName() const;               //nome + numeroSerie

    //getter
    std::string getNome() const;
    int getId() const;
    int getNumeroSerie() const;
    double getPotenza() const;
    bool isSempreAcceso() const;
    int getOrarioAccensione() const;
    int getOrarioSpegnimento() const;
    int getTempoAccensione() const;
    bool isManual() const;
    bool isCP() const;

    //setter
    void setOrarioAccensione(int minuti);   // può lanciare eccezione std::invalid_argument
    void setOrarioSpegnimento(int minuti);  // può lanciare eccezione std::invalid_argument
    void incrementaTempoAccensione(int minuti);  //può lanciare eccezione std::invalid_argument

    //variabili statiche
    static const int MINUTI_GIORNATA = 1440;
    static const int MAX_MINUTI_GIORNATA = 1439;
};

#endif // DISPOSITIVO_H

std::string trasformaOrario(int minuti);
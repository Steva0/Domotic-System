#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <string>

class Dispositivo {
private:
    static int lastId;

protected:
    std::string nome;
    const int id;
    double potenza; // Positivo per produzione, negativo per consumo
    bool sempreAcceso; // Indica se il dispositivo non può mai essere spento
    int orarioAccensione; // In minuti da 0 a 1440
    int orarioSpegnimento; // In minuti da 0 a 1440
    int durata;

public:
    Dispositivo(std::string nom, double pot, bool sempreAcc = false);
    virtual ~Dispositivo() = default;

    double calcolaConsumoEnergetico(int minuti) const; 
    
    std::string getNome() const;
    int getId() const;
    double getPotenza() const;
    bool isSempreAcceso() const;
    int getOrarioAccensione() const;
    int getOrarioSpegnimento() const;
    int getDurata() const;

    void setOrarioAccensione(int minuti);   //può lanciare eccezione std::invalid_argument
    void setOrarioSpegnimento(int minuti);  //può lanciare eccezione std::invalid_argument
    void incrementDurata(int minuti);
};

#endif // DISPOSITIVO_H
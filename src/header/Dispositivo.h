#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <string>
#include <stdexcept>

class Dispositivo {

    friend bool operator==(const Dispositivo&, const Dispositivo&);
    friend std::ostream& operator<<(std::ostream&, const Dispositivo&);

private:
    static int lastId;

protected:
    std::string nome;
    const int id; //Id univoco per ogni dispositivo
    double potenza; // Positivo per produzione, negativo per consumo
    bool sempreAcceso; // Indica se il dispositivo non può mai essere spento
    int orarioAccensione; // In minuti da 0 a 1439
    int orarioSpegnimento; // In minuti da 0 a 1439
    int durataCiclo; //se manuale = 0 se CP != 0
    int tempoAccensione; //tempo tot accensione

public:
    Dispositivo(const std::string& nome, double potenza, int durataCiclo, bool sempreAcceso = false, int orarioAccensione = 0, int orarioSpegnimento = 0); //può lanciare eccezione std::invalid_argument
    virtual ~Dispositivo() = default;

    double calcolaConsumoEnergetico(int minuti) const;

    std::string getNome() const;
    int getId() const;
    double getPotenza() const;
    bool isSempreAcceso() const;
    int getOrarioAccensione() const;
    int getOrarioSpegnimento() const;
    int getTempoAccensione() const;

    void setOrarioAccensione(int minuti);   // può lanciare eccezione std::invalid_argument
    void setOrarioSpegnimento(int minuti);  // può lanciare eccezione std::invalid_argument
    void incrementaTempoAccensione(int minuti);  //può lanciare eccezione std::invalid_argument
};

#endif // DISPOSITIVO_H

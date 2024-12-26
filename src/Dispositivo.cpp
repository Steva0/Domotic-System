#include "Dispostivo.h"
#include <stdexcept>

int Dispositivo::lastId = 0;

Dispositivo::Dispositivo(std::string nom, double pot, bool sempreAcc = false) 
    : nome(nom), 
    potenza(pot),
    sempreAcceso(sempreAcc),
    id(++lastId),
    orarioAccensione(0),
    orarioSpegnimento(0),
    durata(0) {}

double Dispositivo::calcolaConsumoEnergetico(int minuti) const{
    return potenza / 60 * minuti;
}

std::string Dispositivo::getNome() const{
    return nome;
}

int Dispositivo::getId() const{
    return id;
}

double Dispositivo::getPotenza() const{
    return potenza;
}

bool Dispositivo::isSempreAcceso() const{
    return sempreAcceso;
}

int Dispositivo::getOrarioAccensione() const{
    return orarioAccensione;
}

int Dispositivo::getOrarioSpegnimento() const{
    return orarioSpegnimento;
}

int Dispositivo::getDurata() const{
    return durata;
}

void Dispositivo::setOrarioAccensione(int minuti) {
    if (minuti < 0 || minuti >= 1440) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    orarioAccensione = minuti;
}

void Dispositivo::setOrarioSpegnimento(int minuti) {
    if (minuti < 0 || minuti >= 1440) {
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (minuti <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione.");
    }
    orarioSpegnimento = minuti;
}


void Dispositivo::incrementDurata(int minuti) {
    if (minuti < 0) {
        throw std::invalid_argument("Durata non può essere decrementata.");
    }
    durata += minuti;
}

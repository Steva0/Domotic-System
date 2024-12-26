#include "Dispostivo.h"
#include <stdexcept>

Dispositivo::Dispositivo(std::string nom, double pot, bool sempreAcc = false) 
    : nome(nom), 
    potenza(pot),
    sempreAcceso(sempreAcc),
    id(++lastId) {}

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
    return orarioAccensione;
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
    orarioSpegnimento = minuti;
}

void Dispositivo::incrementDurata(int minuti){
    durata += minuti;
}
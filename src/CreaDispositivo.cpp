#include "CreaDispositivo.h"

int CreaDispositivo::lastId = 0;

CreaDispositivo::CreaDispositivo(const std::string& nom, double pot, int durCiclo, bool sempreAcc, int orarioAcc, int orarioSpeg) 
    : nome(nom), 
      id(++lastId),
      potenza(pot),
      sempreAcceso(sempreAcc),
      orarioAccensione(orarioAcc),
      orarioSpegnimento(durCiclo > 0 ? (orarioAcc + durCiclo) % 1440 : orarioSpeg),
      durataCiclo(durCiclo > 0 ? durCiclo : 0),
      durata(0) {
    if (potenza == 0) {
        throw std::invalid_argument("La potenza non può essere zero.");
    }
    if (orarioAccensione < 0 || orarioAccensione >= 1439) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    if (orarioSpegnimento < 0 || orarioSpegnimento >= 1439) {
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (!sempreAcceso && orarioSpegnimento <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione per dispositivi non sempre accesi.");
    }
}

double CreaDispositivo::calcolaConsumoEnergetico(int minuti) const {
    return potenza / 60.0 * minuti;
}

std::string CreaDispositivo::getNome() const {
    return nome;
}

int CreaDispositivo::getId() const {
    return id;
}

double CreaDispositivo::getPotenza() const {
    return potenza;
}

bool CreaDispositivo::isSempreAcceso() const {
    return sempreAcceso;
}

int CreaDispositivo::getOrarioAccensione() const {
    return orarioAccensione;
}

int CreaDispositivo::getOrarioSpegnimento() const {
    return orarioSpegnimento;
}

int CreaDispositivo::getDurata() const {
    return durata;
}

void CreaDispositivo::setOrarioAccensione(int minuti) {
    if (minuti < 0 || minuti >= 1439) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    orarioAccensione = minuti;
    if (!sempreAcceso && orarioSpegnimento <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione per dispositivi non sempre accesi.");
    }
}

void CreaDispositivo::setOrarioSpegnimento(int minuti) {
    if (minuti < 0 || minuti >= 1439) {
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (!sempreAcceso && minuti <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione per dispositivi non sempre accesi.");
    }
    orarioSpegnimento = minuti;
}

void CreaDispositivo::incrementDurata(int minuti) {
    if (minuti < 0) {
        throw std::invalid_argument("Durata non può essere decrementata.");
    }
    durata += minuti;
}

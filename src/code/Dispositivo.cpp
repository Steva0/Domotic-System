#include "../header/Dispositivo.h"

int Dispositivo::lastId = 0;

Dispositivo::Dispositivo(const std::string& nom, double pot, int durCiclo, bool sempreAcc, int orarioAcc, int orarioSpeg) 
    : nome(nom), 
      id(++lastId),
      potenza(pot),
      sempreAcceso(sempreAcc),
      orarioAccensione(orarioAcc),
      orarioSpegnimento(durCiclo > 0 ? (orarioAcc + durCiclo) % 1440 : orarioSpeg),
      durataCiclo(durCiclo > 0 ? durCiclo : 0),
      tempoAccensione(0) {
    if (potenza == 0) {
        throw std::invalid_argument("La potenza non può essere zero.");
    }
    if (orarioAccensione < 0 || orarioAccensione > 1439) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    if (orarioSpegnimento < 0 || orarioSpegnimento > 1439) {
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (orarioSpegnimento == 0 && durataCiclo != 0) {
        orarioSpegnimento = orarioAccensione + durataCiclo;     // caso dispositivo CP
    }
    if (orarioSpegnimento == 0 && durataCiclo == 0) {
        orarioSpegnimento = 1439;                               // caso dispositivo manuale
    }
    if (!sempreAcceso && orarioSpegnimento <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione per dispositivi non sempre accesi.");
    }
}

double Dispositivo::calcolaConsumoEnergetico(int minuti) const {
    return potenza / 60.0 * minuti;
}

std::string Dispositivo::getNome() const {
    return nome;
}

int Dispositivo::getId() const {
    return id;
}

double Dispositivo::getPotenza() const {
    return potenza;
}

bool Dispositivo::isSempreAcceso() const {
    return sempreAcceso;
}

int Dispositivo::getOrarioAccensione() const {
    return orarioAccensione;
}

int Dispositivo::getOrarioSpegnimento() const {
    return orarioSpegnimento;
}

int Dispositivo::getTempoAccensione() const {
    return tempoAccensione;
}

void Dispositivo::setOrarioAccensione(int minuti) {
    if (minuti < 0 || minuti >= 1439) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    orarioAccensione = minuti;
    if (!sempreAcceso && orarioSpegnimento <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione per dispositivi non sempre accesi.");
    }
}

void Dispositivo::setOrarioSpegnimento(int minuti) {
    if (minuti < 0 || minuti >= 1439) {
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (!sempreAcceso && minuti <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione per dispositivi non sempre accesi.");
    }
    orarioSpegnimento = minuti;
}

void Dispositivo::incrementaTempoAccensione(int minuti) {
    if (minuti < 0) {
        throw std::invalid_argument("Tempo accensione non può essere decrementato.");
    }
    tempoAccensione += minuti;
}

//Michele Stevanin
#include "../include/Dispositivo.h"

int Dispositivo::lastId = 0;
std::unordered_map<std::string, int> numeroSerieDispositivi;

Dispositivo::Dispositivo(const std::string& nom, double pot, int durCiclo, bool sempreAcc, int orarioAcc, int orarioSpeg) 
    : nome(nom), 
      id(++lastId),
      numeroSerie(++numeroSerieDispositivi[nome]),
      potenza(pot),
      sempreAcceso(sempreAcc),
      orarioAccensione(orarioAcc),
      orarioSpegnimento(durCiclo > 0 ? (orarioAcc + durCiclo) % MINUTI_GIORNATA : orarioSpeg),
      durataCiclo(durCiclo > 0 ? durCiclo : 0),
      tempoAccensione(0) {
    if (potenza == 0) {
        throw std::invalid_argument("La potenza non può essere zero.");
    }
    if (orarioAccensione < 0 || orarioAccensione > MAX_MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    if (orarioSpegnimento < 0 || orarioSpegnimento > MAX_MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (orarioSpegnimento == 0 && durataCiclo != 0) {
        orarioSpegnimento = orarioAccensione + durataCiclo;     // caso dispositivo CP
    }
    if (orarioSpegnimento == 0 && durataCiclo == 0) {
        orarioSpegnimento = MAX_MINUTI_GIORNATA;                // caso dispositivo manuale
    }
    if (!sempreAcceso && orarioSpegnimento <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione per dispositivi non sempre accesi.");
    }
}

std::string trasformaOrario(int minuti)  {
    int ore = minuti / 60 % 24;
    int min = minuti % 60;
    return (ore < 10 ? "0" : "") + std::to_string(ore) + ":" + (min < 10 ? "0" : "") + std::to_string(min);
}

double Dispositivo::calcolaConsumoEnergetico() const {
    return potenza / 60.0 * tempoAccensione;
}

std::string Dispositivo::showInfo() const{
    std::string info = "Nome: " + getNome() + "\n"
                     + "ID: [ " + std::to_string(id) + " ]\n"
                     + "Tempo di accensione: " + trasformaOrario(tempoAccensione);
    return info;
}

std::string Dispositivo::showAllInfo() const{
    std::string info = "Nome: " + getNome() + "\n"
                     + "ID: [ " + std::to_string(id) + " ]\n"
                     + "Potenza: " + std::to_string(potenza) + " kW\n"
                     + "Sempre acceso: " + (sempreAcceso ? "Si" : "No") + "\n"
                     + "Orario accensione: " + trasformaOrario(orarioAccensione) + "\n"
                     + "Orario spegnimento: " + trasformaOrario(orarioSpegnimento) + "\n"
                     + "Tempo di accensione: " + trasformaOrario(tempoAccensione);
    return info;
}

std::string Dispositivo::showName() const{
    std::string info = "Nome: " + getNome();
    return info;
}

std::string Dispositivo::showSmall() const{
    std::string info = "Nome: " + getNome() + "\n"
                     + "ID: [ " + std::to_string(id)+ " ]";
    return info;
}

std::string Dispositivo::getNome() const {
    if (numeroSerie > 1) {
        return nome + std::to_string(numeroSerie);
    }
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

bool Dispositivo::isManual() const{
    return (durataCiclo == 0);
}
    
bool Dispositivo::isCP() const{
    return (durataCiclo != 0);
}

void Dispositivo::setOrarioAccensione(int minuti) {
    if (minuti < 0 || minuti >= MAX_MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    orarioAccensione = minuti;
    if (!sempreAcceso && orarioSpegnimento <= orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore dell'orario di accensione per dispositivi non sempre accesi.");
    }
}

void Dispositivo::setOrarioSpegnimento(int minuti) {
    if (minuti < 0 || minuti >= MAX_MINUTI_GIORNATA) {
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

bool operator==(const Dispositivo& d1, const Dispositivo& d2) {
    return d1.id == d2.id;
}

std::ostream& operator<<(std::ostream& os, const Dispositivo& dispositivo){
    os << dispositivo.showSmall();
    return os;
}


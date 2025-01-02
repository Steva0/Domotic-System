//Michele Stevanin
#include "../include/Dispositivo.h"

int Dispositivo::lastId = 0;

Dispositivo::Dispositivo() : Dispositivo("", 0, 0, false, 0, 0) {}

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
    if (orarioSpegnimento < orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore o uguale dell'orario di accensione");
    }
}

Dispositivo& Dispositivo::operator=(const Dispositivo& other) {
    if (this != &other) {
        nome = other.nome;
        potenza = other.potenza;
        sempreAcceso = other.sempreAcceso;
        orarioAccensione = other.orarioAccensione;
        orarioSpegnimento = other.orarioSpegnimento;
        durataCiclo = other.durataCiclo;
        tempoAccensione = other.tempoAccensione;
    }
    return *this;
}

std::string trasformaOrario(int minuti)  {
    int ore = minuti / 60 % 24;
    int min = minuti % 60;
    return (ore < 10 ? "0" : "") + std::to_string(ore) + ":" + (min < 10 ? "0" : "") + std::to_string(min);
}

double Dispositivo::calcolaConsumoEnergetico() const {
    return (potenza*tempoAccensione) / 60.0;
}

std::string Dispositivo::showInfo() const{
    std::string info = "Nome: " + getNome() + "\n"
                     + "ID: [ " + std::to_string(id) + " ]\n"
                     + "Tempo di accensione: " + trasformaOrario(tempoAccensione);
    return info;
}

std::string Dispositivo::showAllInfo() const{
    std::string info = "Nome: " + getNome() + "\n\t"
                     + "ID: [ " + std::to_string(id) + " ]\n\t"
                     + "Potenza: " + std::to_string(potenza) + " kW\n\t"
                     + "Sempre acceso: " + (sempreAcceso ? "Si" : "No") + "\n\t"
                     + "Orario accensione: " + trasformaOrario(orarioAccensione) + "\n\t"
                     + "Orario spegnimento: " + trasformaOrario(orarioSpegnimento) + "\n\t"
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

std::string Dispositivo::showConsumo() const{
    std::string info = getNome() + ": " + std::to_string(calcolaConsumoEnergetico()) + " kW";
    return info;
}

std::string Dispositivo::getTipo() const {
    return nome;
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

int Dispositivo::getNumeroSerie() const {
    return numeroSerie;
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

int Dispositivo::getDurataCiclo() const {
    return durataCiclo;
}

bool Dispositivo::isManual() const{
    return (durataCiclo == 0);
}
    
bool Dispositivo::isCP() const{
    return (durataCiclo != 0);
}

void Dispositivo::setTimerOff() {
    if (durataCiclo == 0){
        setOrarioSpegnimento(MAX_MINUTI_GIORNATA);
    }
}

bool Dispositivo::isAcceso(int currentTime) const{
    if (currentTime < 0 || currentTime >= MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    return (getOrarioAccensione() <= currentTime && getOrarioSpegnimento() > currentTime);
}

bool Dispositivo::isGenerator() const{
    return (potenza > 0);
}

void Dispositivo::setOrarioAccensione(int minuti) {
    if (minuti < 0 || minuti >= MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    orarioAccensione = minuti;
    if (orarioSpegnimento < orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore o uguale dell'orario di accensione.");
    }
    orarioSpegnimento = (durataCiclo > 0) ? ((orarioAccensione + durataCiclo) > MINUTI_GIORNATA ? throw std::invalid_argument("Non e' possibile terminare il ciclo in questa giornata!") : orarioAccensione + durataCiclo) : orarioSpegnimento;
}

void Dispositivo::setOrarioSpegnimento(int minuti) {
    if (minuti < 0 || minuti >= MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (minuti < orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore o uguale dell'orario di accensione.");
    }
    orarioSpegnimento = minuti;
}

void Dispositivo::incrementaTempoAccensione(int minuti) {
    if (minuti < 0) {
        throw std::invalid_argument("Tempo accensione non può essere decrementato.");
    }
    tempoAccensione += minuti;
}

void Dispositivo::resetTempoAccensione() {
    tempoAccensione = 0;
}

bool operator==(const Dispositivo& d1, const Dispositivo& d2) {
    return d1.id == d2.id;
}

std::ostream& operator<<(std::ostream& os, const Dispositivo& dispositivo){
    os << dispositivo.showSmall();
    return os;
}


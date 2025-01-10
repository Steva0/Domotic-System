//Michele Stevanin
#include "../include/Dispositivo.h"

/*
Questa classe è molto semplice. L'unica particolarità è che ho deciso di non creare tante classi derivate anche perchè risultavano identiche. 
Ho scelto all'ora di creare una classe unica dispositivo dove la stringa nome identifica il tipo di dispositivo e viene gestito tutto attraverso una sola classe.
La differenza tra Manuali e CP è data dalla durata del ciclo. Inoltre ho deciso di avere una classe così generica per poter gestire una quantità infinita di dispositivi.
Infatti con il costruttore si può creare un dispositivo di qualunque tipo e con qualsiasi caratteristica.
Nel nostro progetto utilizzeremo la classe CreaDispositivo, sempre fatta da me per usare solo i dispositivi presenti all'interno del file ListaDispositivi.h
In questo modo questa classe è molto flessibile e può essere utilizzata in qualsiasi contesto.
*/

int Dispositivo::lastId = 0;
std::unordered_map<std::string, int> Dispositivo::numeroSerieDispositivi;

Dispositivo::Dispositivo() 
    : nome(""), 
      id(0),
      numeroSerie(0),
      potenza(0),
      sempreAcceso(false),
      orarioAccensione(0),
      orarioSpegnimento(0),
      durataCiclo(0),
      tempoAccensione(0),
      timerOn(false) {  }

//Può lanciare eccezione std::invalid_argument
Dispositivo::Dispositivo(const std::string& nom, double pot, int durCiclo, bool sempreAcc, int orarioAcc, int orarioSpeg, bool hasTimer) 
    : nome(nom), 
      id(++lastId),
      numeroSerie(++numeroSerieDispositivi[nome]),
      potenza(pot),
      sempreAcceso(sempreAcc),
      orarioAccensione(orarioAcc),
      orarioSpegnimento(orarioSpeg),
      durataCiclo(durCiclo > 0 ? durCiclo : 0),
      tempoAccensione(0),
      timerOn(hasTimer) {
    if (orarioAccensione < 0 || orarioAccensione > MAX_MINUTI_GIORNATA) {
        --lastId;
        --numeroSerieDispositivi[nome];
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    if (orarioSpegnimento < 0 || orarioSpegnimento > MAX_MINUTI_GIORNATA) {
        --lastId;
        --numeroSerieDispositivi[nome];
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (durataCiclo != 0) {
        orarioSpegnimento = (orarioAccensione + durataCiclo) > MINUTI_GIORNATA ? MAX_MINUTI_GIORNATA : orarioAccensione + durataCiclo;     // caso dispositivo CP
    }
    if (orarioSpegnimento == 0 && durataCiclo == 0) {
        orarioSpegnimento = MAX_MINUTI_GIORNATA;                // caso dispositivo manuale
    }
    if (orarioSpegnimento < orarioAccensione) {
        --lastId;
        --numeroSerieDispositivi[nome];
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore o uguale dell'orario di accensione");
    }
}

Dispositivo& Dispositivo::operator=(const Dispositivo& other) {
    if (this != &other) {
        nome = other.nome;
        id = other.id;
        numeroSerie = other.numeroSerie;
        potenza = other.potenza;
        sempreAcceso = other.sempreAcceso;
        orarioAccensione = other.orarioAccensione;
        orarioSpegnimento = other.orarioSpegnimento;
        durataCiclo = other.durataCiclo;
        tempoAccensione = other.tempoAccensione;
        timerOn = other.timerOn;
    }
    return *this;
}

std::string trasformaOrario(int minuti)  {
    int ore = minuti / 60 % 24;
    int min = minuti % 60;
    return (ore < 10 ? "0" : "") + std::to_string(ore) + ":" + (min < 10 ? "0" : "") + std::to_string(min);
}

//Calcola i kW consumati in totale in kWh
double Dispositivo::calcolaConsumoEnergetico() const {
    return (potenza*tempoAccensione) / 60.0;
}

//Nome + numeroSerie, id, tempoAccensione
std::string Dispositivo::showInfo() const{
    std::string info = "Nome: " + getNome() + "\n"
                     + "ID: [ " + std::to_string(id) + " ]\n"
                     + "Tempo di accensione: " + trasformaOrario(tempoAccensione);
    return info;
}

//Nome + numeroSerie, id,  potenza, sempreAcceso, orarioAccensione, orarioSpegnimento, tempoAccensione
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

//Nome + numeroSerie
std::string Dispositivo::showName() const{
    std::string info = "Nome: " + getNome();
    return info;
}

//Nome + numeroSerie, id
std::string Dispositivo::showSmall() const{
    std::string info = "Nome: " + getNome() + "\n"
                     + "ID: [ " + std::to_string(id)+ " ]";
    return info;
}

//Nome + numeroSerie, consumo energetico
std::string Dispositivo::showConsumo() const{
    std::string info = getNome() + ": " + std::to_string(calcolaConsumoEnergetico()) + " kW";
    return info;
}

//Nome del tipo di dispositivo
std::string Dispositivo::getTipo() const {
    return nome;
}

//Nome + numeroSerie
std::string Dispositivo::getNome() const {
    return nome + "-" + std::to_string(numeroSerie);
}

//Id
int Dispositivo::getId() const {
    return id;
}

//NumeroSerie
int Dispositivo::getNumeroSerie() const {
    return numeroSerie;
}

//Potenza
double Dispositivo::getPotenza() const {
    return potenza;
}

//SempreAcceso (true - false)
bool Dispositivo::isSempreAcceso() const {
    return sempreAcceso;
}

//OrarioAccensione
int Dispositivo::getOrarioAccensione() const {
    return orarioAccensione;
}

//OrarioSpegnimento
int Dispositivo::getOrarioSpegnimento() const {
    return orarioSpegnimento;
}

//TempoAccensione
int Dispositivo::getTempoAccensione() const {
    return tempoAccensione;
}

//DurataCiclo
int Dispositivo::getDurataCiclo() const {
    return durataCiclo;
}

//True se durataCiclo == 0, false se durataCiclo != 0
bool Dispositivo::isManual() const{
    return (durataCiclo == 0);
}

//Rimuove timer di spegnimento, solo ai manuali
void Dispositivo::setTimerOff() {
    if (durataCiclo == 0){
        setOrarioSpegnimento(MAX_MINUTI_GIORNATA);
    }
}

/*True se acceso, false se spento
Può lanciare eccezione std::invalid_argument*/
bool Dispositivo::isAcceso(int currentTime) const{
    if (currentTime < 0 || currentTime >= MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }
    return (getOrarioAccensione() <= currentTime && getOrarioSpegnimento() > currentTime);
}

//True se generatore, false se consumatore
bool Dispositivo::isGenerator() const{
    return (potenza > 0);
}

//Restituisce true se il dispositivo ha un timer attivo
bool Dispositivo::hasTimer() const{
    return timerOn;
}

/*Setta orarioAccensione
Può lanciare eccezione std::invalid_argument*/
void Dispositivo::setOrarioAccensione(int minuti, bool setTimer) {
    if (minuti < 0 || minuti >= MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di accensione non valido.");
    }

    orarioAccensione = minuti;
    orarioSpegnimento = (durataCiclo > 0) ? ((orarioAccensione + durataCiclo) >= MINUTI_GIORNATA ? MAX_MINUTI_GIORNATA : orarioAccensione + durataCiclo) : orarioSpegnimento;
    
    if (orarioSpegnimento < orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore o uguale dell'orario di accensione.");
    }
    timerOn = setTimer;
}

/*Setta orarioSpegnimento
Può lanciare eccezione std::invalid_argument*/
void Dispositivo::setOrarioSpegnimento(int minuti, bool setTimer) {
    if (minuti < 0 || minuti >= MINUTI_GIORNATA) {
        throw std::invalid_argument("Orario di spegnimento non valido.");
    }
    if (minuti < orarioAccensione) {
        throw std::invalid_argument("Orario di spegnimento deve essere maggiore o uguale dell'orario di accensione.");
    }
    orarioSpegnimento = minuti;
    timerOn = setTimer;
}

/*Incrementa tempoAccensione
Può lanciare eccezione std::invalid_argument*/
void Dispositivo::incrementaTempoAccensione(int minuti) {
    if (minuti < 0) {
        throw std::invalid_argument("Tempo accensione non può essere decrementato.");
    }
    tempoAccensione += minuti;
}

 //Resetta tempoAccensione a 0
void Dispositivo::resetTempoAccensione() {
    tempoAccensione = 0;
}

//Setta boolTimer
void Dispositivo::setHasTimer(bool hasTimer) {
    timerOn = hasTimer;
}

//Operatore di uguaglianza
bool operator==(const Dispositivo& d1, const Dispositivo& d2) {
    return d1.id == d2.id;
}

//Operatore di stampa
std::ostream& operator<<(std::ostream& os, const Dispositivo& dispositivo){
    os << dispositivo.showSmall();
    return os;
}

//Fabrizio Scabbia

#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <sys/stat.h>
#include <cmath>
#include <exception>

#include "LinkedListOn.h"
#include "LinkedListProg.h"
#include "LinkedListOff.h"


class Interfaccia
{
private:

    //---------------Variabili d'istanza---------------
    const double MAX_KILOWATT = 3.5;                                                                                            //Potenza massima consumabile dal sistema

    double totalProduced = 0;                                                                                                   //Totale energia prodotta dal sistema
    double totalConsumed = 0;                                                                                                   //Totale energia consumata dal sistema
    int currentSystemTime = 0;                                                                                                  //Orario attuale del sistema (espresso in minuti)

    LinkedListOn dispositiviAccesi = LinkedListOn();                                                                            //DoublyLinkedList contentenente i dispositivi accesi
    LinkedListProg dispositiviProgrammati = LinkedListProg();                                                                   //DoublyLinkedList contentenente i dispositivi programmati
    LinkedListOff dispositiviSpenti = LinkedListOff();                                                                          //DoublyLinkedList contentenente i dispositivi spenti

    std::string nomeFileLog;                                                                                                    //Nome del file di log
    const std::string logDirName = "DomoticSystemLogs";                                                                         //Nome della cartella di output per i file log  
    const std::string possibleCommands[5] = {"set", "rm", "show", "reset", "help"};                                             //Array comandi esistenti


    //---------------Funzioni private---------------

    //  Funzioni eseguite minuto per minuto
    void checkTurnOnDevices();                                                                                                  //Controlla se ci sono dispositivi da accendere in questo momento
    void checkTurnOffDevices();                                                                                                 //Controlla se ci sono dispositivi da spegnere in questo momento
    void updateEnergyUsage();                                                                                                   //Incrementa l'energia totale prodotta e consumata dal sistema
    void checkKilowatt();                                                                                                       //Controlla che l'energia totale del sistema sia inferiore a quella disponibile

    //  Funzioni per accendere o spegnere i dispositivi
    void turnOnDevice(Dispositivo dispositivo);                                                                                 //Accende un dispositivo
    void turnOffDevice(Dispositivo dispositivo, const bool &print = true);                                                      //Spegne un dispositivo
    void changeDeviceStatus(const std::string &nomeDispositivo, const std::string &newStatus);                                  //Cambia lo stato di un dispositivo acceso/spento

    //  Funzioni per gestire timer dei dispositivi
    void setDeviceTimer(Dispositivo& dispositivo, const int &startTime, const int &endTime, const bool &alreadySet = true);     //Imposta il timer per un dispositivo
    void commandSetDeviceTimer(const std::string &nomeDispositivo, const int &startTime, int endTime);                          //Gestisce i timer dei dispositivi
    void handleDeviceHasAlreadyTimer(const std::string &nomeDispositivo, const int &startTime, int endTime);                    //Gestisce il caso in cui si vuole accendere/spegnere un dispositivo che ha già un timer

    //  Funzioni per gestire i comandi inseriti dall'utente
    int handleCommandSet(const std::vector<std::string> &commandVector);                                                        //Gestisce comando set time/device
    int handleCommandSetDevice(const std::vector<std::string> &commandVector);                                                  //Gestisce comando set device
    int handleCommandSetTime(const std::vector<std::string> &commandVector);                                                    //Gestisce comando set time
    int handleCommandRm(const std::vector<std::string> &commandVector);                                                         //Gestisce comando rm
    int handleCommandShow(const std::vector<std::string> &commandVector);                                                       //Gestisce comando show
    std::string showAllDevices();                                                                                               //Restituisce una tabella con tutte le informazioni sui dispositivi disponibili
    int handleCommandReset(const std::vector<std::string> &commandVector);                                                      //Gestisce comando reset time/timers/all

    //  Funzioni per la gestione del file di log
    void initializeFileLog() const;                                                                                             //Inizializza il file di log con data e ora in cui si avvia il programma
    void endFileLog() const;                                                                                                    //Termina il file di log con data e ora in cui si chiude il programma
    void showMessage(const std::string& message, const bool &printToStream = true) const;                                       //Gestisce la scrittura su file e o su outputStream
    
public:
    //---------------Costruttore---------------    
    Interfaccia(std::string logFileName);                                                                                       //Costruttore della classe in cui viene inizializzato il nome del file di log e il file stesso

    //---------------Funzione parse principale---------------
    int parseAndRunCommand(const std::string &userInput);                                                                       //Questa funzione gestisce l'utilizzo dell'interfaccia

    //---------------Funzione per vedere tutti i comandi disponibili---------------
    void showAvailableCommands(std::string message = "") const;                                                                 //Funzione che stampa tutti i possibili comandi con le loro sintassi

    //---------------Distruttore---------------
    ~Interfaccia();                                                                                                             //Distruttore della classe in cui viene terminata la scrittura sul file di log
};

// Eccezione invalidTimeFormat quando viene inserito un orario in un formato errato
class invalidTimeFormat : public std::exception {
public:
    explicit invalidTimeFormat(const std::string& timeType);
    const char* what() const noexcept override;

private:
    std::string message;
};

// Eccezione wrongCommandSyntax lanciata quando viene inserito un comando con una sintassi errata
class wrongCommandSyntax : public std::exception {
public:
    explicit wrongCommandSyntax(const std::string& command);
    const char* what() const noexcept override;

private:
    std::string message;
};

#endif //INTERFACE_H
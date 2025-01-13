#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sys/stat.h>
#include <cmath>


#include "LinkedListOn.h"
#include "LinkedListProg.h"
#include "LinkedListOff.h"



class Interfaccia
{
private:
    const double MAX_KILOWATT = 3.5;

    double totalProduced = 0;
    double totalConsumed = 0;

    void turnOnDevice(Dispositivo dispositivo);
    void turnOffDevice(Dispositivo dispositivo, const bool &print = true);
    void updateEnergyUsage();

    void checkKilowatt();
    void checkTurnOnDevices();
    void checkTurnOffDevices();
    
    void changeDeviceStatus(const std::string &nomeDispositivo, const std::string &newStatus);
    void setDeviceTimer(Dispositivo& dispositivo, const int &startTime, const int &endTime, const bool &alreadySet = true);
    void commandSetDeviceTimer(const std::string &nomeDispositivo, const int &startTime, int endTime);
    void handleDeviceHasAlreadyTimer(const std::string &nomeDispositivo, const int &startTime, int endTime);

    int handleCommandSet(const std::vector<std::string> &commandVector);
    int handleCommandSetDevice(const std::vector<std::string> &commandVector);
    int handleCommandSetTime(const std::vector<std::string> &commandVector);
    int handleCommandRm(const std::vector<std::string> &commandVector);
    int handleCommandShow(const std::vector<std::string> &commandVector);
    int handleCommandReset(const std::vector<std::string> &commandVector);

    void initializeFileLog() const;
    void endFileLog() const;

    void showMessage(const std::string& message, const bool &printToStream = true) const;

    LinkedListOn dispositiviAccesi = LinkedListOn();    
    LinkedListProg dispositiviProgrammati = LinkedListProg();
    LinkedListOff dispositiviSpenti = LinkedListOff();
    
    int currentSystemTime = 0;

    std::string nomeFileLog;
    std::string logDirName = "DomoticSystemLogs";
    std::string possibleCommands[5] = {"set", "rm", "show", "reset", "help"};
    
public:
    int parseAndRunCommand(const std::string &userInput);    
    Interfaccia(std::string logFileName);
    ~Interfaccia();
};

class infoError : public std::exception {
    std::string message;
    public:
        infoError(const std::string& msg) : message(msg) {}
        const char* what() const throw() {
            return message.c_str();
        }
};


#endif
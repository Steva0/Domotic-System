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
    void turnOffDevice(Dispositivo dispositivo, bool print);
    void updateEnergyUsage();

    void checkTurnOnDevices();
    void checkTurnOffDevices();

    void checkKilowatt();
    void changeDeviceStatus(std::string newStatus, std::string nomeDispositivo);
    void setDeviceTimer(Dispositivo& dispositivo, int startTime, int endTime, bool alreadySet = true);
    void commandSetDeviceTimer(int startTime, int endTime, std::string nomeDispositivo);
    void handleDeviceHasAlreadyTimer(std::string nomeDispositivo, int startTime, int endTime);

    int handleCommandSet(const std::vector<std::string> &v);
    int handleCommandSetDevice(const std::vector<std::string> &v);
    int handleCommandSetTime(const std::vector<std::string> &v);
    int handleCommandRm(const std::vector<std::string> &v);
    int handleCommandShow(const std::vector<std::string> &v);
    int handleCommandReset(const std::vector<std::string> &v);

    void initializeFileLog();
    void endFileLog();

    void showMessage(const std::string& message);
    void showMessage(const std::string& message, std::ostream& os);
    void showMessage(const std::string& message, std::ostream& outputStream, std::ofstream& fileStream);

    LinkedListOn dispositiviAccesi = LinkedListOn();    
    LinkedListProg dispositiviProgrammati = LinkedListProg();
    LinkedListOff dispositiviSpenti = LinkedListOff();
    
    int currentSystemTime = 0;

    std::string nomeFileLog;
    std::string logDirName = "DomoticSystemLogs";
    std::string possibleCommands[5] = {"set", "rm", "show", "reset", "help"};
    
public:
    int parseAndRunCommand(std::string command);    
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
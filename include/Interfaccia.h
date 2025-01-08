#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>

#include "LinkedList.h"
#include "LinkedListOff.h"



class Interfaccia
{
private:
    const double MAX_KILOWATT = 3.5;

    double totalProduced = 0;
    double totalUsed = 0;

    void turnOnDevice(Dispositivo dispositivo, int currentTime);
    void turnOffDevice(Dispositivo dispositivo, int currentTime, bool print);
    void updateActiveTime();
    void updateConsumo();

    void checkTurnOnDevices(int currentTime);
    void checkTurnOffDevices(int currentTime);

    void checkKilowatt(int currentTime);
    void changeDeviceStatus(std::string newStatus, std::string nomeDispositivo, int currentTime);
    void setDeviceTimer(Dispositivo& dispositivo, int startTime, int endTime);
    void commandSetDeviceTimer(int startTime, int endTime, std::string nomeDispositivo, int currentTime);
    void handleDeviceHasAlreadyTimer(std::string nomeDispositivo, int startTime, int endTime, int currentTime);

    int handleCommandSet(std::vector<std::string> &v);
    int handleCommandSetDevice(const std::vector<std::string> &v);
    int handleCommandSetTime(const std::vector<std::string> &v);
    int handleCommandRm(const std::vector<std::string> &v);
    int handleCommandShow(const std::vector<std::string> &v);
    int handleCommandReset(const std::vector<std::string> &v);

    void initializeFileLog();
    void endFileLog();

    std::string getCurrentDateTime(bool fileNameCreation=false) const;

    void showMessage(const std::string& message);
    void showMessage(const std::string& message, std::ostream& os);
    void showMessage(const std::string& message, std::ostream& outputStream, std::ofstream& fileStream);
    LinkedList dispositiviAccesi = LinkedList();    
    LinkedList dispositiviProgrammati = LinkedList();
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
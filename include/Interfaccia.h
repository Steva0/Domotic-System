#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "LinkedList.h"
#include "LinkedListOff.h"
class Interfaccia
{
private:
    const double MAX_KILOWATT = 3.5;

    void turnOnDevice(Dispositivo dispositivo, int currentTime);
    void turnOffDevice(Dispositivo dispositivo, int currentTime, bool print);
    void updateActiveTime();

    void checkTurnOnDevices(int currentTime);
    void checkTurnOffDevices(int currentTime);

    void checkKilowatt(int currentTime);
    void changeDeviceStatus(std::string newStatus, std::string nomeDispositivo, int currentTime);
    void setDeviceTimer(Dispositivo& dispositivo, int startTime, int endTime);
    void commandSetDeviceTimer(int startTime, int endTime, std::string nomeDispositivo, int currentTime);
    void handleDeviceHasAlreadyTimer(std::string nomeDispositivo, int startTime, int endTime, int currentTime);
    
    LinkedList dispositiviAccesi = LinkedList();    
    LinkedList dispositiviProgrammati = LinkedList();
    LinkedListOff dispositiviSpenti = LinkedListOff();
    
    int currentTime = 0;

    std::string possibleCommands[5] = {"set", "rm", "show", "reset", "help"};
    
public:    
    int parseAndRunCommand(std::string command);
    Interfaccia();
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
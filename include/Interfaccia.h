#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "LinkedList.h"
#include "LinkedListOff.h"
class Interfaccia
{
private:
    const double MAX_KILOWATT = 3.5;

    void turnOffDevices(LinkedList& dispositiviAccesi, LinkedListOff& dispositiviSpenti, int currentTime, bool& cambiatoQualcosa);
    void checkKilowatt(LinkedList& dispositiviAccesi, LinkedListOff& dispositiviSpenti, int currentTime, bool& blackout, bool& cambiatoQualcosa);
    void changeDeviceStatus(std::string newStatus, std::string nomeDispositivo, LinkedList& dispositiviAccesi, LinkedListOff& dispositiviSpenti, int currentTime);
    void setDeviceTimer(Dispositivo dispositivo, int startTime, int endTime);
    void commandSetDeviceTimer(int startTime, int endTime, std::string nomeDispositivo, LinkedList& dispositiviAccesi, LinkedListOff& dispositiviSpenti, int currentTime);
    void handleDeviceHasAlreadyTimer(std::string nomeDispositivo, int startTime, int endTime, LinkedList& dispositiviAccesi, int currentTime);
    
    int convertTimeToInt(std::string time);
    std::string convertIntToTime(int minuti);
    LinkedList dispositiviAccesi = LinkedList();
    LinkedListOff dispositiviSpenti = LinkedListOff();
    int currentTime = 0;
    std::string possibleCommands[5] = {"set", "rm", "show", "reset", "help"};
    
public:    
    void parseAndRunCommand(std::string command);
    Interfaccia();
    ~Interfaccia();
};


#endif
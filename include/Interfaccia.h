#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "LinkedList.h"
#include "LinkedListOff.h"
class Interfaccia
{
private:    
    int convertTimeToInt(std::string time);
    std::string convertIntToTime(int minuti);
    LinkedList dispositiviAccesi = LinkedList();
    LinkedListOff dispositiviSpenti = LinkedListOff();
    int currentTime = 0;
    std::string possibleCommands[5] = {"set", "rm", "show", "reset", "help"};
    const double MAX_KILOWATT = 3.5;
public:    
    void parseAndRunCommand(std::string command);
    Interfaccia();
    ~Interfaccia();
};


#endif
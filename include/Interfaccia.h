#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "LinkedList.h"
#include "LinkedListOff.h"
class Interfaccia
{
private:    
    int convertTimeToInt(std::string time);
    LinkedList dispositiviAccesi = LinkedList();
    LinkedListOff dispositiviSpenti = LinkedListOff();
    int currentTime = 0;
    std::string possibleCommands[4] = {"set", "rm", "show", "reset"};
public:    
    void parseAndRunCommand(std::string command);

    Interfaccia();
    ~Interfaccia();
};


#endif
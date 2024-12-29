#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "LinkedList.h"
#include "LinkedListOff.h"
class Interfaccia
{
private:    
    int convertTimeToInt(std::string time);
    LinkedList dispositiviAccesi;
    LinkedListOff dispositiviSpenti;
    int currentTime = 0;
public:    
    void parseAndRunCommand(std::string command);

    Interfaccia();
    ~Interfaccia();
};

Interfaccia::Interfaccia(){}

Interfaccia::~Interfaccia(){}
#endif
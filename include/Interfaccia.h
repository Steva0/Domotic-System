#include <string>
//#include <linkedlist-alberto>
//#include <arrayDispositivo-alberto>
class Interfaccia
{
private:    
    int convertTimeToInt(std::string time);
    //LinkedList dispositiviAccesi;
    //arrayDispositivo dispositiviSpenti;
public:    
    void parseAndRunCommand(std::string command);

    Interfaccia();
    ~Interfaccia();
};

Interfaccia::Interfaccia()
{
    //costruttore in cui creerei un puntatore ad ogni struttura (vettore e linked list) per muovermi meglio [da fare]
}

Interfaccia::~Interfaccia()
{
    //distruggo tutto allahakbar
}
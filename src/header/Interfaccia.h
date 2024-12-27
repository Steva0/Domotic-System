#include <string>

class Interfaccia
{
private:
    void parseCommand(std::string command);
    int convertTimeToInt(std::string time);
public:
    Interfaccia();
    ~Interfaccia();
};

Interfaccia::Interfaccia()
{
}

Interfaccia::~Interfaccia()
{
}
 


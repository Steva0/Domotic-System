#include <string>

class Interfaccia
{
private:    
    int convertTimeToInt(std::string time);
public:
    Interfaccia();
    void parseAndRunCommand(std::string command);
    ~Interfaccia();
};

Interfaccia::Interfaccia()
{
}

Interfaccia::~Interfaccia()
{
}
 


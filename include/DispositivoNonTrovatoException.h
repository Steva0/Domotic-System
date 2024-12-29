#ifndef DISPOSITIVONONTROVATOEXCEPTION_H
#define DISPOSITIVONONTROVATOEXCEPTION_H

#include <stdexcept>
#include <string>

class DispositivoNonTrovatoException : public std::invalid_argument
{
    public:
    DispositivoNonTrovatoException(): std::invalid_argument("Dispositivo non trovato!") { }
};

#endif // DISPOSITIVONONTROVATOEXCEPTION_H
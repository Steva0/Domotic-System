#ifndef LISTAVUOTAEXCEPTION_H
#define LISTAVUOTAEXCEPTION_H

#include <stdexcept>

class ListaVuotaException: public std::out_of_range
{
    public:
    ListaVuotaException(): std::out_of_range("La lista è vuota.") { }
};

#endif // LISTAVUOTAEXCEPTION_H
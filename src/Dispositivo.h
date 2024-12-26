#ifndef CREADISPOSITIVO_H 
#define CREADISPOSITIVO_H

#include <string>
#include <stdexcept>
#include <vector>
#include <regex> 
#include "CreaDispositivo.h" 

class Dispositivo {
public:
    // Crea un dispositivo in base al nome, orario di inizio e se si vuole orario di fine
    static CreaDispositivo creaDispositivo(const std::string& nome, int orarioInizio, int orarioFine = -1);

private:
    // Funzione di ricerca che cerca il dispositivo più simile al nome dato, usando espressioni regolari
    static std::string ricercaDispositivoSimile(
        const std::string& query, 
        const std::vector<std::pair<std::string, std::tuple<double, int, bool>>>& dispositivi);
};

#endif // CREADISPOSITIVO_H

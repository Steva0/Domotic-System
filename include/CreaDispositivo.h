//Michele Stevanin
#ifndef CREADISPOSITIVO_H
#define CREADISPOSITIVO_H

#include <string>
#include <stdexcept>
#include "Dispositivo.h"
#include "ListaDispositivi.h"
#include "RicercaDispositivo.h"

class CreaDispositivo {
public:
    // Crea un dispositivo in base al nome, orario di inizio e se si vuole orario di fine
    static Dispositivo* creaDispositivo(const std::string& nome, int orarioInizio, int orarioFine = -1, bool hasTimer = false);  //può causare eccezione std::runtime_error("Dispositivo non trovato!");
};

#endif // CREADISPOSITIVO_H

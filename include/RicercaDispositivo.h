// Michele Stevanin
#ifndef RICERCADISPOSITIVO_H
#define RICERCADISPOSITIVO_H

#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <tuple>
#include <stdexcept>
#include "ListaDispositivi.h"

class RicercaDispositivo {
private:
    static int calcolaDistanza(const std::string &a, const std::string &b);

public:
    static constexpr int soglia = 4; // Soglia di distanza minima accettabile. Semplificando indica il numero di lettere sbagliate massime. Ho trovato che 4 è un buon compromesso

    static std::string ricercaDispositivoSimile(
        const std::string &query,
        const std::vector<std::pair<std::string, std::tuple<double, int, bool>>> &dispositivi); // Cerca un dispositivo simile al nome fornito, utilizzando una regex

    static bool isGenerator(const std::string &nome); // in base al nome fornito dice se è un generatore o meno
};

#endif // RICERCADISPOSITIVO_H

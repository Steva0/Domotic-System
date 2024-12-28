#ifndef RICERCADISPOSITIVO_H
#define RICERCADISPOSITIVO_H

#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <tuple>

class RicercaDispositivo {
private:
    static int calcolaDistanza(const std::string& a, const std::string& b);
public:
    static constexpr int soglia = 4;  // Soglia di distanza minima accettabile. Indica il numero di lettere sbagliate massime. Ho trovato che 4 è un buon valore
    // Cerca un dispositivo simile al nome fornito, utilizzando una regex
    static std::string ricercaDispositivoSimile(
        const std::string& query,
        const std::vector<std::pair<std::string, std::tuple<double, int, bool>>>& dispositivi);
};

#endif // RICERCADISPOSITIVO_H

//Michele Stevanin
#include "../include/RicercaDispositivo.h"

//appunti algoritmi di ricerca fuzzy di informatica 

int RicercaDispositivo::calcolaDistanza(const std::string& a, const std::string& b) {
    std::vector<int> precedente(b.size() + 1), corrente(b.size() + 1);

    for (size_t j = 0; j <= b.size(); ++j) precedente[j] = j;

    for (size_t i = 1; i <= a.size(); ++i) {
        corrente[0] = i;
        for (size_t j = 1; j <= b.size(); ++j) {
            corrente[j] = (a[i - 1] == b[j - 1]) ? precedente[j - 1] : 1 + std::min({precedente[j], corrente[j - 1], precedente[j - 1]});
        }
        std::swap(precedente, corrente);
    }

    return precedente[b.size()];
}

std::string RicercaDispositivo::ricercaDispositivoSimile(
    const std::string& query,
    const std::vector<std::pair<std::string, std::tuple<double, int, bool>>>& dispositivi) {

    std::string miglioreCorrispondenza;
    int distanzaMinima = std::numeric_limits<int>::max();
    
    for (const auto& dispositivo : dispositivi) {
        const std::string& nomeDispositivo = dispositivo.first;

        // Calcola la distanza
        int distanza = calcolaDistanza(query, nomeDispositivo);

        // Aggiorna la migliore corrispondenza se la distanza è inferiore alla distanza minima
        if (distanza < distanzaMinima) {
            distanzaMinima = distanza;
            miglioreCorrispondenza = nomeDispositivo;
        }
    }

    // Se la distanza minima è maggiore della soglia, restituisci una stringa vuota
    if (distanzaMinima > soglia) {
        return "";  // Nessuna corrispondenza significativa
    }

    return miglioreCorrispondenza;
}

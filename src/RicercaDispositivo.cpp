//Michele Stevanin
#include "../include/RicercaDispositivo.h"

/*Quello che fa è calcolare la distanza di Levenshtein tra la stringa di input e ogni stringa
 di dispositivi predefiniti. Esempio Ape e Ave hanno distanza 1. É stata impostata una soglia
  a 4 ovvero ceh se la distanza migliore è maggiore di 4 allora non è stato trovato il dispositivo corretto.*/
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


/*RicercaDispositivoSimile usa al suo interno la funzione calcolaDistanza per vedere 
quale tra tutti i dispositivi nell'array ListaDispositivi ha la distanza minore, 
ovvero è più simile alla parola data e restituisce il suo nome*/
std::string RicercaDispositivo::ricercaDispositivoSimile(
    const std::string& nomeDaCercare,
    const std::vector<std::pair<std::string, std::tuple<double, int, bool>>>& ListaDispositivi) {

    std::string miglioreCorrispondenza;
    int distanzaMinima = std::numeric_limits<int>::max();
    
    for (const auto& dispositivo : ListaDispositivi) {
        const std::string& nomeDispositivo = dispositivo.first;

        // Calcola la distanza
        int distanza = calcolaDistanza(nomeDaCercare, nomeDispositivo);

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


/*RicercaDispositivoSimile usa al suo interno la funzione calcolaDistanza per vedere quale tra tutti i 
dispositivi nell'array ListaDispositivi ha la distanza minore, ovvero è più simile alla parola data e 
controlla se la sua potenza è negativa (consumatore -> false) o positiva (generatore -> true).
Può lanciare eccezione std::runtime_error*/
bool RicercaDispositivo::isGenerator(const std::string& nomeDispositivo){
    std::string nomeDispositivoPulito = "";
    for (const auto& c : nomeDispositivo) {
        if (c != '-') {
            nomeDispositivoPulito += c;
        } else {
            break;
        }
    }
    
    std::string nomeCorretto = RicercaDispositivo::ricercaDispositivoSimile(nomeDispositivoPulito, dispositiviPredefiniti);

    for (const auto& dispositivo : dispositiviPredefiniti) {
        if (dispositivo.first == nomeCorretto) {
            auto [potenza, durataCiclo, sempreAcceso] = dispositivo.second;

            bool isGenerator = potenza > 0;

            return isGenerator;
        }
    }
    throw std::runtime_error("Dispositivo non trovato!");
}

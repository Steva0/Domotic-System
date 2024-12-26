#include "CreaDispositivo.h"
#include <algorithm>  // Per std::find_if

CreaDispositivo CreaDispositivo::creaDispositivo(const std::string& nome, int orarioInizio, int orarioFine) {
    // Insieme dei dispositivi con le loro caratteristiche
    static const std::vector<std::pair<std::string, std::tuple<double, int, bool>>> dispositivi = {
        {"Impianto fotovoltaico", {1.5, 0, false}},  
        {"Lavatrice", {-2.0, 110, false}},         
        {"Lavastoviglie", {-1.5, 195, false}},     
        {"Pompa di calore + termostato", {-2.0, 0, false}},  
        {"Tapparelle elettriche", {-0.3, 1, false}},  
        {"Scaldabagno", {-1.0, 0, false}},  
        {"Frigorifero", {-0.4, 0, true}},  
        {"Forno a microonde", {-0.8, 2, false}},  
        {"Asciugatrice", {-0.5, 60, false}},  
        {"Televisore", {-0.2, 60, false}}  
    };

    // Trova il dispositivo più simile al nome cercato (considerando anche variazioni di nome)
    std::string nomeTrovato = ricercaDispositivoSimile(nome, dispositivi);

    // Se troviamo un dispositivo simile, recuperiamo le caratteristiche e creiamo l'oggetto
    auto it = std::find_if(dispositivi.begin(), dispositivi.end(),
        [&nomeTrovato](const auto& pair) {
            return pair.first == nomeTrovato; // Confronta il nome trovato con quello nel catalogo
        });

    if (it != dispositivi.end()) {
        const auto& [nomeDispositivo, caratteristiche] = *it;
        const auto& [potenza, durataCiclo, sempreAcceso] = caratteristiche;
        
        // Se il dispositivo è sempre acceso, imposta orarioInizio e orarioFine
        if (sempreAcceso) {
            orarioInizio = 0; // Inizia all'inizio della giornata
            orarioFine = 1439; // Ultimo minuto della giornata
        }
        // Se la durata del dispositivo è 0 (manuale), imposta orarioFine a 1439 se non fornito
        else if (durataCiclo == 0 && orarioFine == -1) {
            orarioFine = 1439; // Resta acceso tutto il giorno
        }
        // Se la durata è maggiore di 0 (ciclo prefissato), calcola orarioFine come orarioInizio + durataCiclo
        else if (durataCiclo > 0) {
            orarioFine = orarioInizio + durataCiclo; // Calcola il tempo di fine in base al ciclo
        }

        // Crea e restituisce il dispositivo
        return Dispositivo(nomeDispositivo, potenza, durataCiclo, sempreAcceso, orarioInizio, orarioFine);
    } else {
        // Se il dispositivo non viene trovato, solleva un'eccezione
        throw std::invalid_argument("Dispositivo non trovato: " + nome);
    }
}

std::string CreaDispositivo::ricercaDispositivoSimile(
    const std::string& query, 
    const std::vector<std::pair<std::string, std::tuple<double, int, bool>>>& dispositivi) {
    
    // Crea un'espressione regolare per la query (ricerca case-insensitive)
    std::regex regexQuery(query, std::regex_constants::icase);

    // Cerca tra tutti i dispositivi se uno corrisponde alla query
    for (const auto& dispositivo : dispositivi) {
        const auto& [nomeDispositivo, _] = dispositivo;

        // Se il nome del dispositivo corrisponde alla query tramite regex, restituisci il nome del dispositivo
        if (std::regex_search(nomeDispositivo, regexQuery)) {
            return nomeDispositivo; // Trovato, restituisce il nome del dispositivo
        }
    }
    
    // Se non viene trovato nessun dispositivo, restituisci una stringa vuota
    return ""; 
}

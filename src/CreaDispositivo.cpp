//Michele Stevanin
#include "../include/CreaDispositivo.h"

/*
Ho scelto di creare questa classe ausiliaria per facilitare e automatizzare la creazione di un dispositivo.
Tutto quello di cui ha bisogno è il nome del dispositivo, l'orario di accensione e l'orario di spegnimento opzionalmente.
In automatico usando la classe RicercaDispositivo e la lista dei dispositivi, cerca il dispositivo simile,
ne prende i valori e crea il dispositivo.
*/


//Può lanciare eccezione std::runtime_error
Dispositivo* CreaDispositivo::creaDispositivo(const std::string& nome, int orarioInizio, int orarioFine, bool hasTimer) {
    // Usa la classe RicercaDispositivo per trovare il dispositivo simile
    std::string dispositivoSimile = RicercaDispositivo::ricercaDispositivoSimile(nome, dispositiviPredefiniti);

    // Cerca il dispositivo nella lista dei dispositivi
    for (const auto& dispositivo : dispositiviPredefiniti) {
        if (dispositivo.first == dispositivoSimile) {
            auto [potenza, durataCiclo, sempreAcceso] = dispositivo.second;

            // Logica per determinare orarioInizio e orarioFine
            if (sempreAcceso) {
                orarioFine = Dispositivo::MAX_MINUTI_GIORNATA;  // Resta acceso fino all'ultimo minuto della giornata
            } else if (durataCiclo == 0 && orarioFine == -1) {
                orarioFine = Dispositivo::MAX_MINUTI_GIORNATA;  // Se manuale e orarioFine non specificato, resta acceso fino alla fine della giornata
            } else if (durataCiclo > 0) {
                orarioFine = (orarioInizio + durataCiclo) > Dispositivo::MINUTI_GIORNATA ? Dispositivo::MAX_MINUTI_GIORNATA : orarioInizio + durataCiclo;    // Per dispositivi a ciclo prefissato
            }

            // Crea e restituisce il dispositivo
            return new Dispositivo(dispositivoSimile, potenza, durataCiclo, sempreAcceso, orarioInizio, orarioFine, hasTimer);
        }
    }

    throw std::runtime_error("Dispositivo non trovato!");
}

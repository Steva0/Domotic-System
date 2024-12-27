#include "../header/CreaDispositivo.h"

Dispositivo CreaDispositivo::creaDispositivo(const std::string& nome, int orarioInizio, int orarioFine) {
    // Usa la classe RicercaDispositivo per trovare il dispositivo simile
    std::string dispositivoSimile = RicercaDispositivo::ricercaDispositivoSimile(nome, dispositiviPredefiniti);

    // Cerca il dispositivo nella lista dei dispositivi
    for (const auto& dispositivo : dispositiviPredefiniti) {
        if (dispositivo.first == dispositivoSimile) {
            auto [potenza, durataCiclo, sempreAcceso] = dispositivo.second;

            // Logica per determinare orarioInizio e orarioFine
            if (sempreAcceso) {
                orarioFine = Dispositivo::MAX_MINUTI_GIORNATA; // Resta acceso fino all'ultimo minuto della giornata
            } else if (durataCiclo == 0 && orarioFine == -1) {
                orarioFine = Dispositivo::MAX_MINUTI_GIORNATA; // Se manuale e orarioFine non specificato, resta acceso fino alla fine della giornata
            } else if (durataCiclo > 0) {
                orarioFine = orarioInizio + durataCiclo; // Per dispositivi a ciclo prefissato
            }

            // Crea e restituisce il dispositivo
            return Dispositivo(dispositivoSimile, potenza, durataCiclo, sempreAcceso, orarioInizio, orarioFine);
        }
    }

    throw std::runtime_error("Dispositivo non trovato!");
}

#include <iostream>
#include "../header/CreaDispositivo.h"

int main() {
    try {
        // Test 1: Creazione di un dispositivo sempre acceso
        Dispositivo dispositivo1 = CreaDispositivo::creaDispositivo("televsoe", 0);
        std::cout << "Dispositivo creato: " << dispositivo1.getNome() << "\n"
                  << "ID: " << dispositivo1.getId() << "\n"
                  << "Potenza: " << dispositivo1.getPotenza() << " W\n"
                  << "Sempre acceso: " << (dispositivo1.isSempreAcceso() ? "Si" : "No") << "\n"
                  << "Orario accensione: " << dispositivo1.getOrarioAccensione() << " min\n"
                  << "Orario spegnimento: " << dispositivo1.getOrarioSpegnimento() << " min\n";

        // Test 2: Creazione di un dispositivo a ciclo prefissato
        Dispositivo dispositivo2 = CreaDispositivo::creaDispositivo("Lavatrice", 480);
        std::cout << "\nDispositivo creato: " << dispositivo2.getNome() << "\n"
                  << "ID: " << dispositivo2.getId() << "\n"
                  << "Potenza: " << dispositivo2.getPotenza() << " W\n"
                  << "Sempre acceso: " << (dispositivo2.isSempreAcceso() ? "Si" : "No") << "\n"
                  << "Orario accensione: " << dispositivo2.getOrarioAccensione() << " min\n"
                  << "Orario spegnimento: " << dispositivo2.getOrarioSpegnimento() << " min\n";

        // Test 3: Creazione di un dispositivo manuale
        Dispositivo dispositivo3 = CreaDispositivo::creaDispositivo("impiato fotovolaico", 600, 1200);
        std::cout << dispositivo3 << "\n";

        // Test 4: Creazione con un nome non presente
        Dispositivo dispositivo4 = CreaDispositivo::creaDispositivo("NonEsistente", 300);
        std::cout << "\nDispositivo creato: " << dispositivo4.getNome() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "\nErrore: " << e.what() << "\n";
    }
    
    return 0;
}

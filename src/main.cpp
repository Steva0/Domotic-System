#include <iostream>
#include "../include/CreaDispositivo.h"

int main() {

    try {
        // Test 1: Creazione di un dispositivo sempre acceso
        Dispositivo* dispositivo1 = new Dispositivo(CreaDispositivo::creaDispositivo("televsoe", 0));
        std::cout << *dispositivo1 << "\n\n";

        // Test 2: Creazione di un dispositivo a ciclo prefissato
        Dispositivo* dispositivo2 = new Dispositivo(CreaDispositivo::creaDispositivo("frigorifo", 480));
        std::cout << *dispositivo2 << "\n\n";

        // Test 3: Creazione di un dispositivo manuale
        Dispositivo* dispositivo3 = new Dispositivo(CreaDispositivo::creaDispositivo("televisore", 600, 1200));
        std::cout << *dispositivo3 << "\n\n";

        
        //Questo è un esempio mio di codice che non funziona. Lo metto in un try catch separato per non far fallire i test
        try {
            // Test 4: Creazione con un nome non presente
            Dispositivo* dispositivo4 = new Dispositivo(CreaDispositivo::creaDispositivo("NonEsistente", 300));
            std::cout << "\nDispositivo creato: " << dispositivo4->getNome() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "\nErrore: " << e.what() << "\n";
        }
        

        // Deallocazione della memoria
        delete dispositivo1;
        delete dispositivo2;
        delete dispositivo3;

        std::cout << "\nDovrei aver fatto tutto giusto ;)\n";

    } catch (const std::exception& e) {
        std::cerr << "\nErrore: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
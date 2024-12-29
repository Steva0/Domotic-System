#include <iostream>
#include "../include/Interfaccia.h"

int main() {
    std::cout << "Benvenuto nel interfaccia di gestione del sistema domotico!\n";
    std::cout << "Per uscire dal programma, scrivi 'esci'.\n";
    Interfaccia interface;
    while (true) {
        std::string comando;
        std::cout << "> " << std::flush;
        std::getline(std::cin, comando);

        if (comando == "esci" || comando == "exit" || comando == "q") {
            break;
        }
        std::cout << "Comando inserito: " << comando << std::endl; //debug
        // Esegui il comando usando le funzioni definite in Interfaccia.cpp
        interface.parseAndRunCommand(comando);
    }

    return 0;
}
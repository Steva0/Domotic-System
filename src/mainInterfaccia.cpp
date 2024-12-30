#include <iostream>
#include "../include/Interfaccia.h"

int main() {
    std::cout << "Benvenuto nel interfaccia di gestione del sistema domotico!\n";
    std::cout << "Per uscire dal programma, scrivi 'esci'.\n";
    Interfaccia interface;
    interface.parseAndRunCommand("help");
    while (true) {
        std::string comando;
        std::cout << "> " << std::flush;
        std::getline(std::cin, comando);

        if (comando == "esci" || comando == "exit" || comando == "q") {
            break;
        }
        
        interface.parseAndRunCommand(comando);
    }

    return 0;
}
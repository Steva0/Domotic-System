#include <iostream>
#include "../include/Interfaccia.h"

int main() {
    std::cout << "Benvenuto nel interfaccia di gestione del sistema domotico!\n";
    std::cout << "Per uscire dal programma, scrivi 'esci'.\n";
    Interfaccia interface;
    interface.parseAndRunCommand("help");
    bool fromFile = false;
    while (true) {
        std::string comando;
        std::cout << "> " << std::flush;
        std::getline(std::cin, comando);

        if (comando == "esci" || comando == "exit" || comando == "q") {
            break;
        }
        if(comando == "fromFile"){
            fromFile = true;
            continue;
        }
        if(fromFile) std::cout << comando << std::endl; //per leggere meglio l'output
        interface.parseAndRunCommand(comando);
        std::cin.clear();
    }

    return 0;
}
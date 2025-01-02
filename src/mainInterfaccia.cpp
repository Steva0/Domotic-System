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

        if(fromFile) std::cout << comando << std::endl; //per leggere meglio l'output

        int status = interface.parseAndRunCommand(comando);

        std::cin.clear();

        if (status == -1) {
            break;
        }else if (status == 12345){
            fromFile = true;
        }
    }

    return 0;
}
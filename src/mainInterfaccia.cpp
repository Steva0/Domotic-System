#include <iostream>
#include "../include/Interfaccia.h"

int main() {
    std::cout << "Benvenuto nel interfaccia di gestione del sistema domotico!\n";
    std::cout << "Per uscire dal programma, scrivi 'esci'.\n";
    Interfaccia interface;

    try{
        interface.parseAndRunCommand("help");
    }catch (const infoError& e){
        std::cout << e.what() << std::endl;
    }

    bool fromFile = false;

    while (true) {
        try{
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
                std::cout << std::endl;
            }
        }catch (const infoError& e){
            std::cout << e.what() << std::endl;
        }
        catch (const std::exception& e){
            std::cout << "Errore Lanciato: " << e.what() << std::endl;
        }
    }

    return 0;
}
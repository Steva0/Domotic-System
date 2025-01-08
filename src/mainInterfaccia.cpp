#include <iostream>
#include "../include/Interfaccia.h"
#include <string>
#include <iostream>
#include <cstdlib> // Per la funzione system()

void clearTerminal() {
    #ifdef _WIN32
        system("cls"); // Comando per Windows
    #else
        system("clear"); // Comando per Unix/Linux/MacOS
    #endif
}

bool isValidFileName(const std::string& filename) {
    const std::string invalidChars = "\\/:*?\"<>|"; // Caratteri vietati su Windows, alcuni sono permessi su Linux
    
    if (filename.empty() || filename.size() > 255) {
        return false;
    }

    // Controllo ogni carattere
    for (char c : filename) {
        if (invalidChars.find(c) != std::string::npos || c == '\0') {
            return false;
        }
    }

    // Controllo se termina con '/' o è solo "."
    if (filename == "." || filename == ".." || filename.back() == '/') {
        return false;
    }

    return true;
}


int main(int argc, char* argv[]) {
    const std::string estensione = ".txt";
    std::string fileName = "DefaultFileName";

    clearTerminal();

    if (argc > 1) {
        fileName = argv[1];
        for (int i = 2; i < argc; i++) {
            fileName += " ";
            fileName += argv[i];
        }

        if(!isValidFileName(fileName)){
            std::cout << "Errore. Il nome fornito per il file di log non e' valido." << std::endl;
        }else if (fileName.size() >= estensione.size()){
            if (!(fileName.compare(fileName.size() - estensione.size(), estensione.size(), estensione) == 0)){
                fileName+= ".txt";
            }
        }
    }

    if(fileName == "DefaultFileName"){
        bool rispostaOk = false;
        bool nomeFileValido = false;
        std::string risposta;
        std::string userInsertedFileName;

        do{
            //clearTerminal();
            std::cout << "Vuoi dare un nome specifico al file di log? [y/n] ";
            std::getline(std::cin, risposta);

            if(risposta == "y" || risposta == "yes" || risposta == "si" || risposta == "Y"){
                rispostaOk = true;

                std::cout << "Inserisci un nome valido per il file di log: ";

                std::getline(std::cin, userInsertedFileName);

                if(!isValidFileName(userInsertedFileName)){
                    std::cout << "Errore. Il nome fornito per il file di log non e' valido." << std::endl;
                }else if (userInsertedFileName.size() >= estensione.size()){
                    if (!(userInsertedFileName.compare(userInsertedFileName.size() - estensione.size(), estensione.size(), estensione) == 0)){
                        userInsertedFileName += ".txt";
                    }
                    nomeFileValido = true;
                    fileName = userInsertedFileName;
                    clearTerminal();
                }
            }else if(risposta == "n" || risposta == "N" || risposta == "no"){
                rispostaOk = true;
                nomeFileValido = true;
                clearTerminal();
                std::cout << "Verra' usato un nome generico per il file di log." << std::endl << std::endl;
                fileName = "-Log";
            }else{
                std::cout << "Risposta non valida." << std::endl;
            }

        }while(!rispostaOk || !nomeFileValido);
    }

    std::cout << "Benvenuto nel interfaccia di gestione del sistema domotico!\n";
    std::cout << "Per uscire dal programma, scrivi 'esci'.\n";

    Interfaccia interface(fileName);

    try{
        interface.parseAndRunCommand("help");
    }catch (const infoError& e){
        std::cout << e.what() << std::endl;
    }

    bool fromFile = false;

    while (true) {
        try{
            std::string comando;
            std::cout << ">> " << std::flush;
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
#include <iostream>
#include <string>
#include <cstdlib> // Per la funzione system()
#include "../include/Interfaccia.h"

// Funzione per pulire il terminale e avere un output migliore
void clearTerminal() {
    #ifdef _WIN32
        system("cls"); // Comando per Windows
    #else
        system("clear"); // Comando per Unix/Linux/MacOS
    #endif
}

// Funzione che verifica se il nome per il file di log scelto dall'utente è valido
bool isValidFileName(const std::string& filename) {
    // Caratteri vietati su Windows, alcuni sono permessi su Linux
    const std::string invalidChars = "\\/:*?\"<>|"; 

    // Se il nome del file di log voluto è troppo lungo o è vuoto non è valido
    if (filename.empty() || filename.size() > 255) {
        return false;
    }

    // Controllo che ogni carattere sia valido
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

std::string parseFileName(const int &argCount, const char argVector[]){
    const std::string estensione = ".txt";
    std::string fileName = "DefaultFileName";
    std::string tempFileName;

    if (argCount > 1) {
        tempFileName = argVector[1];
        for (int i = 2; i < argCount; i++) {
            tempFileName += " ";
            tempFileName += argVector[i];
        }

        if(!isValidFileName(tempFileName)){
            std::cout << "Errore. Il nome fornito per il file di log non e' valido.\n";

        }else if (tempFileName.size() >= estensione.size()){
            if (!(tempFileName.compare(tempFileName.size() - estensione.size(), estensione.size(), estensione) == 0)){
                tempFileName+= ".txt";
            }
            fileName = tempFileName;
        }
    }

    if(fileName == "DefaultFileName"){
        bool rispostaOk = false;
        bool nomeFileValido = false;
        std::string risposta;
        std::string userInsertedFileName;

        do{
            std::cout << "Vuoi dare un nome specifico al file di log? [y/N] ";
            std::getline(std::cin, risposta);

            if(risposta == "y" || risposta == "yes" || risposta == "si" || risposta == "Y"){
                rispostaOk = true;

                std::cout << "Inserisci un nome valido per il file di log: ";

                std::getline(std::cin, userInsertedFileName);

                if(!isValidFileName(userInsertedFileName)){
                    std::cout << "Errore. Il nome fornito per il file di log non e' valido.\n";
                }else if (userInsertedFileName.size() >= estensione.size()){

                    // Controllo se l'utente ha già inserito l'estensione .txt, e se non la ha inserita la aggiungo alla fine del fileName
                    if (!(userInsertedFileName.compare(userInsertedFileName.size() - estensione.size(), estensione.size(), estensione) == 0)){
                        userInsertedFileName += ".txt";
                    }
                    nomeFileValido = true;
                    fileName = userInsertedFileName;
                    clearTerminal();
                }
            }else if(risposta == "n" || risposta == "N" || risposta == "no" || risposta ==""){
                rispostaOk = true;
                nomeFileValido = true;
                clearTerminal();
                std::cout << "Verra' usato un nome generico per il file di log.\n\n";
                fileName = "-Log";
            }else{
                std::cout << "Risposta non valida.\n";
            }
        }while(!rispostaOk || !nomeFileValido);
    }
    return fileName;
}

int main(int argc, char* argv[]) {
    
    clearTerminal();

    std::string fixedFileName = parseFileName(argc, *argv);

    std::cout << "Benvenuto nel interfaccia di gestione del sistema domotico!\n";
    std::cout << "Per uscire dal programma, scrivi 'esci'.\n";

    Interfaccia interface(fixedFileName);

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

            //per leggere meglio l'output quando la modalità fromFile è attiva, stampo anche il comando inserito dall'utente
            if(fromFile){
                std::cout << comando << std::endl; 
            } 
           
            int status = interface.parseAndRunCommand(comando);
            
            std::cin.clear();

            if (status == -1) { // codice di ritorno per terminare il programma
                break;
            }else if (status == 12345){ // codice di ritorno per abilitare la modalità fromFile
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
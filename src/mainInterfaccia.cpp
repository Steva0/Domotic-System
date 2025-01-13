#include <iostream>
#include <string>
#include <cstdlib> // Per la funzione system()
#include "../include/Interfaccia.h"

// Funzione per pulire il terminale e avere un output più pulito
int clearTerminal() {
    #ifdef _WIN32
        return system("cls"); // Comando per Windows
    #else
        return system("clear"); // Comando per Unix/Linux/MacOS
    #endif
}

// Funzione che verifica se il nome per il file di log scelto dall'utente è valido
bool isValidFileName(const std::string& filename) {
    const std::string invalidChars = "\\/:*?\"<>|";                                     // Caratteri vietati su Windows, alcuni sono permessi su Linux

    // Se il nome del file di log voluto è troppo lungo o è vuoto non è valido
    if (filename.empty() || filename.size() > 255) {
        return false;
    }

    // Controllo che non ci siano caratteri non validi
    for (char c : filename) {
        if (invalidChars.find(c) != std::string::npos || c == '\0') {
            return false;
        }
    }

    // Controllo che il fileName non termini con '/' oppure sia solo "."
    if (filename == "." || filename == ".." || filename.back() == '/') {
        return false;
    }
    return true;
}

/**
    Questa funzione analizza gli argomenti passati al programma per determinare il nome del file di log.
    Se non viene fornito un nome valido, chiede all'utente di inserirne uno o usa un nome generico.
    Aggiunge l'estensione ".txt" se non è già presente.
*/
std::string parseFileName(const int &argCount, char* argVector[]){
    const std::string estensione = ".txt";
    std::string fileName = "DefaultFileName";
    std::string tempFileName;

    if (argCount > 1) {// Se il programma viene lanciato con almeno un argomento
        tempFileName = argVector[1];
        for (int i = 2; i < argCount; i++) {    //Concateno tutti gli argomenti in una unica variabile
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

//Se il filename è rimasto lo stesso, o il fileName inserito tramite argomenti non andava bene oppure non è stato inserito affatto, 
//quindi chiedo all'utente di scegliere un nome per il file di log
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

                    // Controllo se l'utente ha già inserito l'estensione .txt, e se non la ha inserita la concateno alla fine del fileName
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

    std::string fixedFileName = parseFileName(argc, argv);  //Genero il fileName per il file di log

    std::cout << "Benvenuto nel interfaccia di gestione del sistema domotico!\n";
    std::cout << "Per uscire dal programma, scrivi 'esci'.\n";

    Interfaccia interface(fixedFileName);

    interface.showAvailableCommands();   //In questo modo appena il programma si avvia vengono visualizzati subito i comandi disponibili con le relative sintassi    

    bool fromFile = false;  //Modalita' fromFile

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
                std::cout << "Modalita' fromFile attivata\n";
            }

        }
        catch (const std::exception& e){
            std::cout << "Errore Lanciato: " << e.what() << std::endl;
        }
    }
    return 0;
}
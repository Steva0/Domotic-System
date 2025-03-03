// Fabrizio Scabbia

/*
 * Funzionalità principali della classe Interfaccia:
 * - Gestione di dispositivi (accensione, spegnimento, timer, e stato) rispettando il limite energetico impostato.
 * - Implementazione di comandi per l'interazione con l'utente (set, rm, show, reset).
 * - Controllo della sintassi dei comandi e gestione delle eccezioni (es. formato orario, comandi errati).
 * - Log automatico di tutte le operazioni su file e organizzazione dei file di log in una unica cartella.
 * - Conversione e gestione dei tempi (formati hh:mm, minuti totali, ore/minuti).
 * - Integrazione di limiti energetici per spegnere dispositivi non strettamente necessari in caso di superamento della capacità massima.
 * - Supporto per la gestione più dispositivi dello stesso tipo utilizzando il serial number.
 */

#include "../include/Interfaccia.h"

// Implementazione dell'eccezione invalidTimeFormat con informazioni sull'orario sbagliato
invalidTimeFormat::invalidTimeFormat(const std::string& timeType)
    : message("Formato orario [" + timeType + "] non valido! (hh:mm)") {}

const char* invalidTimeFormat::what() const noexcept {
    return message.c_str();
}

// Implementazione dell'eccezione wrongCommandSyntax con messaggio la sintassi del corretta del comando inserito 
wrongCommandSyntax::wrongCommandSyntax(const std::string& command) {
    if (command == "set time") {
        message = "Sintassi corretta: set time ${TIME}\n";
    } else if (command == "set device") {
        message = "Sintassi corretta: set $DEVICE_NAME on/off\n"
                  "                   set $DEVICE_NAME ${START} [${STOP}]\n";
    } else if (command == "rm") {
        message = "Sintassi corretta: rm $DEVICE_NAME\n";
    } else if (command == "reset") {
        message = "Sintassi corretta: reset time/timers/all\n";
    } else if (command == "set") {
        message = "Sintassi corretta: set time ${TIME}\n"
                  "Sintassi corretta: set $DEVICE_NAME on/off\n"
                  "                   set $DEVICE_NAME ${START} [${STOP}]\n";
    } else {
        message = "Comando sconosciuto.";
    }
}

const char* wrongCommandSyntax::what() const noexcept {
    return message.c_str();
}

// Crea una directory ricevendo come parametro il nome di essa
bool createDirectory(const std::string& folderName) {
#if defined(_WIN32) || defined(_WIN64)
    std::string command = "mkdir \"" + folderName + "\"";
#else
    std::string command = "mkdir -p \"" + folderName + "\"";
#endif
    return std::system(command.c_str()) == 0;
}

// Riceve come parametro il nome di una cartella e verifica se essa esiste o meno
bool directoryExists(const std::string& dirName) {
    struct stat info;
    return (stat(dirName.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

// Operatore di somma tra due vettori di tipo dispositivo
std::vector<Dispositivo> operator+(std::vector<Dispositivo>&& array1, std::vector<Dispositivo>&& array2) {
    std::vector<Dispositivo> result = array1;
    
    // Inserisco alla fine del vettore tutto il secondo vettore
    result.insert(result.end(), array2.begin(), array2.end());
    return result;
}

//Converte il tempo da formato hh:mm in numero di minuti
int convertTimeToInt(std::string time) {
    std::string s;
    std::stringstream ss(time);
    std::vector<std::string> v;
    bool explicitTime = false;

    for(char c : time) {
        if(c == ':') {
            explicitTime = true;
            break;
        }else if(!isdigit(c)) {
            return -1;
        }
    }
    
    if(explicitTime) {
        while (getline(ss, s, ':')) {
            v.push_back(s);
        }
        int hours = std::stoi(v.at(0));
        int minutes = std::stoi(v.at(1));
        if(hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
            return -1;
        }
        return hours * 60 + minutes;
    }else{ 
        int hours = std::stoi(time);
        if(hours < 0 || hours > 23) {
            return -1;
        }
        return hours * 60;
    }
}

// Converte numero di minuti in formato hh:mm
std::string convertIntToTime(int minuti) {
    int ore = minuti / 60 % 24;
    int min = minuti % 60;
    return (ore < 10 ? "0" : "") + std::to_string(ore) + ":" + (min < 10 ? "0" : "") + std::to_string(min);
}

// Converte numero di minuti in formato n. ore e n. minuti
std::string convertIntToTimeFormat(int minutiCount) {
    std::ostringstream risultato;
    int ore = minutiCount / 60;
    int minuti = minutiCount % 60;
    if (ore > 0) {
        risultato << ore << (ore == 1 ? " ora" : " ore");
        if (minuti > 0) {
            risultato << " e ";
        }
    }
    if (minuti > 0) {
        risultato << minuti << (minuti == 1 ? " minuto" : " minuti");
    }
    return risultato.str();
}

// Ritorna data e ora in formato Y-M-D HH:MM:SS o Y-M-D HH_MM_SS se fileCreation è true
std::string getCurrentDateTime(bool fileNameCreation=false) {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[100];
    if(fileNameCreation) {
        std::strftime(buffer, sizeof(buffer), "%d-%m-%y %H_%M_%S", localTime);
        return buffer;
    }
    std::strftime(buffer, sizeof(buffer), "%d-%m-%y %H:%M:%S", localTime);
    return buffer;
}

/*
Questa funzione permette all'utente di inserire il nome di un dispositivo anche se esso contiene spazi.
Riceve un vettore contenente tutte le parole dall'input separate da spazi e restituisce un nuovo vettore in cui
il nome del dispositivo occupa una sola cella pur avendo spazi al suo interno.
*/
std::vector<std::string> parseInputString(const std::vector<std::string> inputArray, const std::string& command) {
    std::vector<std::string> outputArray;
    std::string deviceName = "";
    int inputSize = inputArray.size();
    if (inputSize == 1) {
        return inputArray;
    }

    //Devo gestire solamente i casi in cui il comando è set (ma non set time), rm, oppure show (senza altri argomenti)
    if ((command == "set" && inputArray.at(1) != "time") || command == "rm" || (command == "show" && inputArray.size() > 1)) {
        int index = 1;
        for (; index < inputSize; index++) {
            if (command == "set" && (isdigit(inputArray.at(index).at(0)) || inputArray.at(index) == "on" || inputArray.at(index) == "off")) {
                break;
            }
            deviceName += inputArray.at(index) + " ";
        }

        deviceName = deviceName.substr(0, deviceName.size() - 1); // Tolgo spazio finale che viene aggiunto prima in ogni caso
        outputArray.push_back(command);
        outputArray.push_back(deviceName);

        if (command == "set" && inputArray.at(1) != "on" && inputArray.at(1) != "off") {
            for (int i = index; i < inputSize; i++) {
                outputArray.push_back(inputArray.at(i));
            }
        } else if (command == "set") {
            outputArray.push_back(inputArray.at(index));
        }
    } else {
        return inputArray;
    }
    return outputArray;
}

// Aggiunge al nome dato il serial number del dispositivo in modo da poter gestire più dispositivi con lo stesso nome
std::string fixDeviceName(std::string userInputName) {
    bool hasSerial = false;
    std::string deviceName;
    std::string serialNumber;
    for(char c: userInputName) {
        if(hasSerial) {
            serialNumber += c;
        }else{
            deviceName += c;
        }
        if(c=='-') hasSerial=true;
    }
    deviceName = RicercaDispositivo::ricercaDispositivoSimile(deviceName, dispositiviPredefiniti);
    if(hasSerial) return deviceName + "-" + serialNumber;
    return deviceName + "-1";
}

// Gestisce gli eventuali errori di sintassi del comando da parte dell'utente lanciando un eccezione contentente la giusta sintassi del comando
void incompleteOrWrongCommand(const std::string& command) {
    throw wrongCommandSyntax(command);      //fare catch nel main
}

// Controlla che il tempo inserito sia nel formato corretto hh:mm
void checkWrongTimeFormat(const std::string &timeType, const int &time) {
    if(time == -1) {
        throw invalidTimeFormat(timeType);
    }
}

// Controlla se esistono dispositivi da accendere e in tal caso li accende spostandoli dalla lista dei programmati in quella degli accesi
void Interfaccia::checkTurnOnDevices() {
    try{        
        std::vector<Dispositivo> dispositiviTempAccesi = dispositiviProgrammati.removeDevicesToPowerOn(currentSystemTime);
        for(Dispositivo dispositivo : dispositiviTempAccesi) {
            turnOnDevice(dispositivo);
        }
    }catch(const std::out_of_range &e) {}
}

// Controlla se esistono dispositivi da spegnere e in tal caso li spegne spostandoli dalla lista degli accesi in quella degli spenti
void Interfaccia::checkTurnOffDevices() {
    try{
        std::vector<Dispositivo> dispositiviTempSpenti = dispositiviAccesi.removeDevicesToPowerOff(currentSystemTime);
        for(Dispositivo dispositivo : dispositiviTempSpenti) {
            turnOffDevice(dispositivo);
        }
    }catch(const std::out_of_range &e) {}
}

// Incrementa il consumo e la produzione totale del sistema
void Interfaccia::updateEnergyUsage() {
    totalProduced += dispositiviAccesi.producedEnergy();
    totalConsumed += dispositiviAccesi.consumedEnergy();
}

/*
    Controlla che il consumo totale attuale (consumata - prodotta) sia inferiore al numero di kilowatt disponibili. 
    In caso negativo spegne il primo dispositivo che non sia un generatore e che non sia un dispositivo che deve essere 
    sempre acceso fino a quando il consumo attuale diventa < dei kilowatt disponibili
*/
void Interfaccia::checkKilowatt() {
    bool removed = false;
    std::vector<std::string> dispositiviSpentiString;

    while(dispositiviAccesi.getCurrentConsumption() + MAX_KILOWATT < 0) {// se ho superato i kilowatt tolgo il primo dispositivo che non sia sempre acceso
        if(!removed) {
            showMessage("E' stato superato il limite di kilowatt.");
            removed = true;
        }       
        Dispositivo disp = dispositiviAccesi.removeFirst();
        disp.setOrarioSpegnimento(currentSystemTime);
        turnOffDevice(disp, false);
        dispositiviSpentiString.push_back(disp.getNome());
    }

    if(removed) {
        if(dispositiviSpentiString.size() == 1) {
            showMessage("Il dispositivo " + dispositiviSpentiString.at(0) + " si e' spento.");
            return;
        }
        std::cout << "Dispositivi spenti: ";
        for(int i=0; i<dispositiviSpentiString.size()-1; i++) {
            std::cout << dispositiviSpentiString.at(i) << ", ";
        }
        std::cout << dispositiviSpentiString.at(dispositiviSpentiString.size()-1) << std::endl;
        std::cout << "Consumo attuale: " << std::to_string(dispositiviAccesi.getCurrentConsumption()) << "kWh.\n";
    }
}

// Funzione che mette nella lista "dispositiviAccesi" un dispositivo e stampa a schermo e sul file che esso si è acceso
void Interfaccia::turnOnDevice(Dispositivo dispositivo) {
    dispositiviAccesi.insert(dispositivo);
    showMessage("Il dispositivo " + dispositivo.getNome() + " si e' acceso.");
    checkKilowatt();
}

// Funzione che mette nella lista "dispositiviSpenti" un dispositivo e stampa a schermo e sul file che esso si è spento
void Interfaccia::turnOffDevice(Dispositivo dispositivo, const bool &print) {
    dispositiviSpenti.insert(dispositivo);
    if(print) {
        showMessage("Il dispositivo " + dispositivo.getNome() + " si e' spento.");
    }
}

/*
    Cambia lo stato di un dispositivo identificato da un nome, se il dispositivo non è mai stato acceso o programmato viene creato un nuovo dispositivo
    altrimenti il dispositivo viene tolto dalla lista in cui è attualmente e viene modificato il tempo di accensione (e di spegnimento nel caso di un dispositivo cp).
    Infine il dispositivo viene spostato nella lista corretta (accesi o spenti)
*/
void Interfaccia::changeDeviceStatus(const std::string &nomeDispositivo, const std::string &newStatus) {
    if(newStatus == "on") { //accendo il dispositivo
        if(dispositiviAccesi.contains(nomeDispositivo)) {
            std::cout << "Il dispositivo " + nomeDispositivo + " e' gia' acceso! ";

            std::string risposta;
            bool rispostaOk = false;
            do{
                std::cout << "Vuoi creare un nuovo dispositivo? [y/N] ";
                std::getline(std::cin, risposta);
                if(risposta == "n" || risposta == "N" || risposta == "no" || risposta == "") {
                    rispostaOk = true;  //non faccio nulla
                }else if(risposta == "y" || risposta == "Y" || risposta == "yes") {
                    rispostaOk = true;
                    Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, currentSystemTime);
                    turnOnDevice(*dispositivo);
                }else{ 
                    std::cout << "Risposta non valida, riprova.\n";
                }
            }while(!rispostaOk);

        }else{
            if(dispositiviProgrammati.contains(nomeDispositivo)){
                Dispositivo dispositivo = dispositiviProgrammati.removeDispositivo(nomeDispositivo);
                std::cout << "Il dispositivo " + dispositivo.getNome() + " e' gia' programmato per accendersi alle " + convertIntToTime(dispositivo.getOrarioAccensione()) + ".\n";
                std::string risposta;
                bool rispostaOk = false;
                do {
                    std::cout << "Vuoi accenderlo ora? In caso di risposta negativa verra' creato un nuovo dispositivo dello stesso tipo [Y/n] ";
                    std::getline(std::cin, risposta);
                    if(risposta == "y" || risposta == "Y" || risposta == "yes" || risposta == "") {
                        rispostaOk = true;
                        dispositivo.setOrarioAccensione(currentSystemTime, false);
                        turnOnDevice(dispositivo);
                        dispositivo.setHasTimer(false);
                    }else if(risposta == "n" || risposta == "N" || risposta == "no") {
                        dispositiviProgrammati.insert(dispositivo);
                        rispostaOk = true;
                        Dispositivo* dispositivoNew = CreaDispositivo::creaDispositivo(nomeDispositivo, currentSystemTime);
                        turnOnDevice(*dispositivoNew);
                        showMessage("E' stato creato il nuovo dispostivo " + dispositivo.getNome());
                    }else{
                        std::cout << "Risposta non valida, riprova.\n";
                    }
                } while (!rispostaOk);
            }else if(dispositiviSpenti.contains(nomeDispositivo)) {
                Dispositivo dispositivo = dispositiviSpenti.removeDispositivo(nomeDispositivo);
                if(dispositivo.isManual()) {
                    dispositivo.setTimerOff();
                    dispositivo.setOrarioAccensione(currentSystemTime);
                }else{ 
                    dispositivo.setOrarioAccensione(currentSystemTime);
                }
                turnOnDevice(dispositivo);
            }else{
                Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, currentSystemTime);
                turnOnDevice(*dispositivo);
            }
        }
    }else if(newStatus == "off") { //spengo il dispositivo
        if(dispositiviAccesi.contains(nomeDispositivo)) {
            Dispositivo dispositivo = dispositiviAccesi.removeDispositivo(nomeDispositivo);
            dispositivo.setOrarioSpegnimento(currentSystemTime);
            turnOffDevice(dispositivo);
            if(dispositivo.hasTimer()){
                dispositivo.setHasTimer(false);
            }
        }else{
            if(dispositiviProgrammati.contains(nomeDispositivo)) {
                throw std::invalid_argument("Il dispositivo " + nomeDispositivo + " si deve ancora accendere! Per rimuovere il timer usare rm.");
            }
            if(dispositiviSpenti.contains(nomeDispositivo)) {
                throw std::invalid_argument("Il dispositivo " + nomeDispositivo + " e' gia' spento!");
            }
            throw std::invalid_argument("Dispositivo inesistente!");
        }
    }
}

// Imposta un timer al dispositivo che viene passato tramite argomento e stampa l'orario di accensione e spegnimento
void Interfaccia::setDeviceTimer(Dispositivo& dispositivo, const int &startTime, const int &endTime, const bool &alreadySet) {
    if(!alreadySet) {
        if(!dispositivo.isManual()) {
            dispositivo.setOrarioAccensione(startTime);
        }else{
            dispositivo.setOrarioSpegnimento(endTime);
            dispositivo.setOrarioAccensione(startTime);
        }
    }
    dispositivo.setHasTimer(true);
    showMessage("Impostato un timer per il dispositivo " + dispositivo.getNome() + " dalle " + convertIntToTime(startTime) + " alle " + convertIntToTime(dispositivo.getOrarioSpegnimento()));
    if(currentSystemTime == startTime) {
        turnOnDevice(dispositivo);
    } else {
        dispositiviProgrammati.insert(dispositivo);
    }
}

/*
    Imposta il timer ad un dispositivo gestendo i vari casi:
    -se il dispositivo non è mai stato usato, allora lo crea
    -se il dispositivo non ha un timer ed è spento allora imposta il timer
    -se il dispositivo è acceso o ha già un timer impostato viene lanciata la funzione handleDeviceHasAlreadyTimer() che chiede all'utente cosa vuole fare
*/
void Interfaccia::commandSetDeviceTimer(const std::string &nomeDispositivo, const int &startTime, int endTime) {
    if(dispositiviAccesi.contains(nomeDispositivo) || dispositiviProgrammati.contains(nomeDispositivo)) {
        handleDeviceHasAlreadyTimer(nomeDispositivo, startTime, endTime);
    }else if(dispositiviSpenti.contains(nomeDispositivo)) {
        Dispositivo dispositivo = dispositiviSpenti.removeDispositivo(nomeDispositivo);        
        if(dispositivo.isManual() && endTime == -1){
            endTime = Dispositivo::MAX_MINUTI_GIORNATA;
        }        
        setDeviceTimer(dispositivo, startTime, endTime, false);
        endTime = dispositivo.getOrarioSpegnimento();
    }else{
        Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime, true);
        endTime = dispositivo->getOrarioSpegnimento();
        setDeviceTimer(*dispositivo, startTime, endTime);
    }
}

/*
    Gestisce il caso in cui si voglia modificare il timer di un dispositivo che ha già un timer chiedendo all'utente cosa vuole fare:
    creare un nuovo dispositivo dello stesso tipo ma con nome diverso avente come timer quello scelto dall'utente
    oppure sovrascrivere il timer del dispositivo con il timer scelto dall'utente spegnendolo se necessario
*/
void Interfaccia::handleDeviceHasAlreadyTimer(const std::string &nomeDispositivo, const int &startTime, int endTime) {
    std::cout << "Il dispositivo " << nomeDispositivo << " ha gia' un timer!\n";
    bool rispostaOk = false;
    std::string risposta;
    do{
        std::cout << "Vuoi sovrascrivere il timer? In caso di risposta negativa verra' creato un nuovo dispositivo [y/N] ";
        std::getline(std::cin, risposta);
        if(risposta == "n" || risposta == "N" || risposta == "no" || risposta == "") {  //creo un nuovo dispositivo 
            rispostaOk = true;
            Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime, true);
            showMessage("E' stato creato il nuovo dispostivo " + dispositivo->getNome());
            setDeviceTimer(*dispositivo, startTime, endTime);
        }else if(risposta == "y" || risposta == "Y" || risposta == "yes") { //sovrascrivo il timer
            rispostaOk = true;
            Dispositivo dispositivo;
            if(dispositiviProgrammati.contains(nomeDispositivo)) {//il dispositivo deve ancora accendersi
                dispositivo = dispositiviProgrammati.removeDispositivo(nomeDispositivo);
            }else{
                dispositivo = dispositiviAccesi.removeDispositivo(nomeDispositivo);
            }
            if(dispositivo.isManual() && endTime == -1){
                endTime = Dispositivo::MAX_MINUTI_GIORNATA;
            }
            if(currentSystemTime >= dispositivo.getOrarioAccensione()){
                showMessage("Il dispositivo " + nomeDispositivo + " si e' spento.");
            }
            setDeviceTimer(dispositivo, startTime, endTime, false);
        }else{ 
            std::cout << "Risposta non valida, riprova.\n";
        }
    }while(!rispostaOk);
}

/*
    Controlla la sintassi e la gestione del comando generale SET
    Decide se l'utente vuole cambiare il tempo usando SET TIME oppure vuole impostare un timer o accendere un dispositivo con SET DEVICE
*/
int Interfaccia::handleCommandSet(const std::vector<std::string> &commandVector)
{
    if(commandVector.size() < 2) {
        incompleteOrWrongCommand("set");
        return 1;
    }
    std::string arg = commandVector.at(1);
    if(arg == "time") {
        return handleCommandSetTime(commandVector);
    }else{ 
        return handleCommandSetDevice(commandVector);
    }
}

/*
    Controlla la sintassi e la gestione del comando SET DEVICE con il quale l'utente può:
        -Accendere o spegnere un dispositivo con SET DEVICE ON/OFF
        -Impostare un timer di un dispositivo con SET DEVICE START [STOP], in questo caso controlla anche la validità dei tempi inseriti
*/
int Interfaccia::handleCommandSetDevice(const std::vector<std::string> &commandVector) {

    if(commandVector.size() < 3) {
        incompleteOrWrongCommand("set device");
        return 1;
    }
    std::string nomeDispositivo = fixDeviceName(commandVector.at(1));
    std::string arg2 = commandVector.at(2);

    if(arg2 == "on" || arg2 == "off") { // accendo o spengo il dispositivo
        changeDeviceStatus(nomeDispositivo, arg2);
    }else{ // imposto il timer di un dispositivo
        int startTime = -1;
        try{
            startTime = convertTimeToInt(arg2);
        }catch(const std::exception &e) {
            incompleteOrWrongCommand("set device");
            return 1;
        }

        checkWrongTimeFormat("startTime", startTime);

        int endTime = -1;
        if(commandVector.size() == 4) { // controllo se l'utente ha inserito un tempo di spegnimento
            try{
                endTime = convertTimeToInt(commandVector.at(3));
            }catch(const std::exception &e) {
                incompleteOrWrongCommand("set device");
                return 1;
            }
            checkWrongTimeFormat("endTime", endTime);
        }

        if(currentSystemTime > startTime) {
            throw std::invalid_argument("Non puoi programmare un dispositivo per il passato!");
        }
        if(endTime <= startTime && endTime != -1){
            throw std::invalid_argument("Il tempo di spegnimento non puo' essere minore o uguale al tempo di accensione.");
        }
        commandSetDeviceTimer(nomeDispositivo, startTime, endTime);
    }
    return 0;
}

/*
    Controlla la sintassi e la gestione del comando SET TIME e verifica la correttezza dell'orario inserito.
    Fa scorrere il tempo minuto per minuto fino al tempo voluto e ogni minuto effettua le seguenti operazioni:
        -Aumenta il tempo di accensione di ogni dispositivo
        -Aggiorna il consumo e la produzione di energia totale
        -Controlla se ci sono dispositivi da spegnere
        -Controlla se ci sono dispositivi da accendere
        -Controlla che il consumo totale non superi il numero di kilowatt disponibili
*/
int Interfaccia::handleCommandSetTime(const std::vector<std::string> &commandVector) {
    if(commandVector.size() < 3) {
        incompleteOrWrongCommand("set time");
        return 1;
    }

    int wantedTime = convertTimeToInt(commandVector.at(2));

    checkWrongTimeFormat("wantedTime", wantedTime);

    if(currentSystemTime > wantedTime) {
        throw std::invalid_argument("Non puoi tornare indietro nel tempo!");
    }

    while (currentSystemTime < wantedTime) {
        ++currentSystemTime;
        // aggiorno il tempo di accensione, aumento consumo, controllo se ci sono dispositivi da spegnere
        if(!dispositiviAccesi.isEmpty()) {
            dispositiviAccesi.incrementTimeOn();
            updateEnergyUsage();
            checkTurnOffDevices();
        }

        // controllo se ci sono dispositivi da accendere
        if(!dispositiviProgrammati.isEmpty()) {
            checkTurnOnDevices();
        }

        // controllo di non aver superato i kilowatt
        if(!dispositiviAccesi.isEmpty()) {
            checkKilowatt();
        }
    }
    showMessage("L'orario attuale e' " + convertIntToTime(currentSystemTime));
    return 0;
}

// Controlla la sintassi e la gestione del comando RM eseguendo la richiesta di rimozione del timer  fatta dall'utente
int Interfaccia::handleCommandRm(const std::vector<std::string> &commandVector) {
    if(commandVector.size() < 2) {
        incompleteOrWrongCommand("rm");
        return 1;
    }

    std::string nomeDispositivo = fixDeviceName(commandVector.at(1));
    if(dispositiviAccesi.contains(nomeDispositivo)) {
        dispositiviAccesi.removeTimer(nomeDispositivo, currentSystemTime);        
    }else if(dispositiviProgrammati.contains(nomeDispositivo)) {
        dispositiviProgrammati.removeTimer(nomeDispositivo, currentSystemTime);
        Dispositivo dispositivo = dispositiviProgrammati.removeDispositivo(nomeDispositivo);
        dispositiviSpenti.insert(dispositivo);
    }else if(dispositiviSpenti.contains(nomeDispositivo)) {
        std::invalid_argument("Il dispositivo non ha un timer!");
    }
    else{
        std::invalid_argument("Dispositivo inesistente!");
    }
    showMessage("Rimosso il timer per il dispositivo " + nomeDispositivo);
    return 0;
}

// Controlla la sintassi e la gestione del comando SHOW mostrando i consumi totali del sistema o per dispositivo
int Interfaccia::handleCommandShow(const std::vector<std::string> &commandVector) {
    if(commandVector.size() < 2) {
        std::ostringstream message;
        message << std::fixed << std::setprecision(3);

        message << "Attualmente il sistema ha prodotto " << totalProduced << "kWh e ha consumato " << totalConsumed << "kWh. Nello specifico:\n\t";
        if(dispositiviAccesi.isEmpty() && dispositiviProgrammati.isEmpty() && dispositiviSpenti.isEmpty()) {
            message << "Il sistema non sta gestendo alcun dispositivo.";
        }else{
            message << dispositiviAccesi.showAll();
            if(!dispositiviAccesi.isEmpty() && (!dispositiviProgrammati.isEmpty() || !dispositiviSpenti.isEmpty())){
                message << "\n\t";
            }
            message << dispositiviProgrammati.showAll(currentSystemTime);
            if(!dispositiviProgrammati.isEmpty() && !dispositiviSpenti.isEmpty()){
                message << "\n\t";
            }
            message << dispositiviSpenti.showAll();
        }
        showMessage(message.str());
    }else if(commandVector.size() == 2) {
        std::string argNome = commandVector.at(1);

        if(argNome == "debug") {
            std::string message;
            message += "\nACCESI\n" + dispositiviAccesi.showAllDebug();
            message += "\nPROGRAMMATI\n" + dispositiviProgrammati.showAllDebug();
            message += "\nSPENTI\n" + dispositiviSpenti.showAllDebug();
            showMessage(message);
            return 0;
        }

        if (argNome == "devices"){
            std::cout << showAllDevices();
            return 0;
        }

        std::string nomeDispositivo = fixDeviceName(argNome);
        std::ostringstream message;
        message << std::fixed << std::setprecision(3);
        message << "Il dispositivo " << nomeDispositivo << " ha attualmente " << (RicercaDispositivo::isGenerator(nomeDispositivo) ? "prodotto " : "consumato ");

        if(dispositiviAccesi.contains(nomeDispositivo)) {
            message << std::to_string(std::fabs(dispositiviAccesi.show(nomeDispositivo))) << "kWh.";
        }else if(dispositiviProgrammati.contains(nomeDispositivo)) {
            message << std::to_string(std::fabs(dispositiviProgrammati.show(nomeDispositivo))) << "kWh.";
        }else if(dispositiviSpenti.contains(nomeDispositivo)) {
            message << std::to_string(std::fabs(dispositiviSpenti.show(nomeDispositivo))) << "kWh.";
        }else{ 
            throw std::invalid_argument("Il dispositivo " + nomeDispositivo + " non e' mai stato acceso o programmato.");
        }

        showMessage(message.str());
    }else{ 
        incompleteOrWrongCommand("show");
        return 1;
    }
    return 0;    
}

std::string Interfaccia::showAllDevices()
{
    std::ostringstream message;

    message << "\nLista Dispositivi:\n";

    // Calcolare la lunghezza massima per ciascuna colonna
    size_t maxNomeLength = 0;
    size_t maxPotenzaLength = 0;
    size_t maxDurataCicloLength = 0;
    size_t maxSempreAccesoLength = 13; // "Sempre Acceso" ha una lunghezza fissa

    // Determinare la lunghezza massima di ogni colonna
    for (const auto &dispositivo : dispositiviPredefiniti)
    {
        const std::string nome = dispositivo.first;
        const auto &[potenza, durataCiclo, sempreAcceso] = dispositivo.second;

        maxNomeLength = std::max(maxNomeLength, nome.length());
        maxPotenzaLength = std::max(maxPotenzaLength, std::to_string(potenza).length());
        maxDurataCicloLength = std::max(maxDurataCicloLength, (durataCiclo == 0 ? 7 : convertIntToTimeFormat(durataCiclo).length()));
    }

    // Stampa l'intestazione della tabella con la larghezza dinamica delle colonne
    message << std::left
            << std::setw(maxNomeLength) << "Nome"
            << std::right // "Appoggio a destra la potenza e la durata ciclo"
            << std::setw(maxPotenzaLength + 3) << "Potenza"
            << std::setw(maxDurataCicloLength + 3) << "Durata Ciclo\n";

    message << std::string(maxNomeLength + maxPotenzaLength + maxDurataCicloLength + maxSempreAccesoLength + 12, '-') << "\n";

    // Stampare i dati
    for (const auto &dispositivo : dispositiviPredefiniti)
    {
        const auto &nome = dispositivo.first;
        const auto &[potenza, durataCiclo, sempreAcceso] = dispositivo.second;

        // Nome dispositivo
        message << std::left << std::setw(maxNomeLength) << nome;

        // Campo "Potenza"
        message << std::right << std::setw(maxPotenzaLength);
        std::ostringstream potenzaStream;
        potenzaStream << std::fixed << std::setprecision(3);
        if (potenza >= 0)
        {
            potenzaStream << "+"; // Aggiungi il simbolo "+" solo per i numeri positivi
        }
        potenzaStream << potenza;
        message << potenzaStream.str() << " kW"; // Aggiungi la stringa completa formattata

        // Campo "Durata Ciclo"
        message << std::right << std::setw(maxDurataCicloLength + 3);
        if (durataCiclo == 0)
        {
            message << "Manuale";
        }
        else
        {
            message << convertIntToTimeFormat(durataCiclo);
        }

        // Sempre acceso
        if (sempreAcceso)
        {
            message << std::setw(maxSempreAccesoLength + 3) << "Sempre Acceso";
        }

        message << "\n";
    }
    return message.str();
}
/*
    Controlla la sintassi e la gestione del comando generale RESET e dei 3 possibili casi:
        -RESET TIME
        -RESET TIMERS
        -RESET ALL
*/
int Interfaccia::handleCommandReset(const std::vector<std::string> &commandVector) {
    if(commandVector.size() < 2) {
        incompleteOrWrongCommand("reset");
        return 1;
    }
    std::string arg = commandVector.at(1);
    if(arg == "time") {
        // riporto tempo a 00:00, tutti i dispositivi alle condizioni iniziali (?), i timer vengono mantenuti
        currentSystemTime = 0;
        totalProduced = 0;
        totalConsumed = 0;

        std::vector<Dispositivo> tempDevices = dispositiviAccesi.removeAll() + dispositiviProgrammati.removeAll() + dispositiviSpenti.removeAll();

        for (Dispositivo dispositivo : tempDevices) {
            dispositivo.resetTempoAccensione();
            if(dispositivo.hasTimer()) {
                dispositiviProgrammati.insert(dispositivo);
            }else{ 
                dispositiviSpenti.insert(dispositivo);
            }
        }
        showMessage("L'orario attuale e' " + convertIntToTime(currentSystemTime));
    }else if(arg == "timers") {
        // tolgo tutti i timer impostati e i dispositivi restano nello stato corrente (acceso/spento)
        dispositiviAccesi.removeAllTimers(currentSystemTime);
        std::vector<Dispositivo> removed = dispositiviProgrammati.removeAll();
        for (Dispositivo dispositivo : removed) {
            dispositiviSpenti.insert(dispositivo);
        }
        dispositiviSpenti.removeAllTimers(currentSystemTime);
    }else if(arg == "all") {
        // riporto tutto alle condizioni iniziali (orario a 00:00, tolgo tutti i timer, tutti i dispositivi spenti)
        currentSystemTime = 0;
        totalProduced = 0;
        totalConsumed = 0;

        std::vector<Dispositivo> tempDevices = dispositiviAccesi.removeAll() + dispositiviProgrammati.removeAll() + dispositiviSpenti.removeAll();

        for (Dispositivo dispositivo : tempDevices) {
            dispositivo.resetTempoAccensione();
            dispositiviSpenti.insert(dispositivo);
        }

        dispositiviSpenti.resetAll();
    }
    return 0;
}

// Inizializza il file di log data e ora nel momento in cui il programma è stato avviato
void Interfaccia::initializeFileLog() const {
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if(logFile.is_open()) {
        logFile << "Programma avviato alle: " << getCurrentDateTime() << "\n";
        logFile.close();
    }
}

// Scrive sul file di log data e ora nel momento in cui il programma è stato terminato
void Interfaccia::endFileLog() const {
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if(logFile.is_open()) {
        logFile << "Programma terminato alle: " << getCurrentDateTime() << "\n\n";
        logFile.close();
    }
}

// Stampa a schermo e sul file log i comandi eseguiti e i messaggi destinati all'utente
void Interfaccia::showMessage(const std::string& message, const bool &printToStream) const {
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if(printToStream){
        std::string formattedMessage = "[" + convertIntToTime(currentSystemTime) + "] " + message;
        std::cout << formattedMessage << std::endl;
        if(logFile.is_open()) {
            logFile << formattedMessage << std::endl;
        }
    }else{
        if(logFile.is_open()) {
            logFile << message << std::endl;
        }
    }    
}

// Controlla la sintassi del comando inserito dall'utente e in base al comando inserito lancia la funzione per gestire correttamente il comando 
int Interfaccia::parseAndRunCommand(const std::string &userInput) {

    if(userInput =="") return 0;
    if(userInput == "fromFile") return 12345;   // Attivo modalità fromFile
    if(userInput == "esci" || userInput == "exit" || userInput == "q") return -1;   // Comando di exit

    std::string s;
    std::stringstream ss(userInput);
    std::vector<std::string> v;

    // Divido l'input in argomenti separati da spazi
    while (getline(ss, s, ' ')) {
        v.push_back(s);
    }

    std::string command = v.at(0);

    bool commandOk = false;

    // Controllo se il comando inserito dall'utente è uno dei comandi possibili
    for(std::string s:possibleCommands) {
        if(s == command) {
            commandOk = true;
            break;
        }
    }

    v = parseInputString(v, command);

    if(!commandOk) {
        showAvailableCommands("Comando non valido!\n");
        return 1;
    }

    if(command == "help") {
        showAvailableCommands();
        return 0;
    }

    showMessage("Comando inserito: " + userInput, false);
    showMessage("L'orario attuale e' " + convertIntToTime(currentSystemTime));
    if(command == "set") {
        return handleCommandSet(v);
    }
    else if(command == "rm") {
        return handleCommandRm(v);
    }
    else if(command == "show") {
        return handleCommandShow(v);
    }
    else if(command == "reset") {
        return handleCommandReset(v);
    }
    return 0;
}

//Funzione che stampa tutti i possibili comandi con le relative sintassi
void Interfaccia::showAvailableCommands(std::string message) const{
        message += "Comandi disponibili:\n";
        message += "\thelp                                  Mostra questa lista di aiuto\n";
        message += "\tset time ${TIME}                      Cambia il tempo del sistema\n";
        message += "\tset $DEVICE_NAME on/off               Accende o spegne un dispositivo\n";
        message += "\tset $DEVICE_NAME ${START} [${STOP}]   Imposta un timer ad un dispositivo\n";
        message += "\trm $DEVICE_NAME                       Toglie il timer ad un dispositivo\n";
        message += "\tshow                                  Visualizza le statistiche di consumo del sistema\n";
        message += "\tshow devices                          Visualizza tutti i dispositivi con tutte le informazioni associate\n";
        message += "\tshow $DEVICE_NAME                     Visualizza il consumo di un dispositivo\n";
        message += "\tshow debug                            (Debug) Mostra le liste dei dispositivi\n";
        message += "\treset time                            (Debug) Riporta il sistema alle condizioni iniziali conservando i timer\n";
        message += "\treset timers                          (Debug) Resetta tutti i timer impostati ai dispositivi\n";
        message += "\treset all                             (Debug) Riporta il sistema alle condizioni iniziali rimuovendo anche i timer\n";
        std::cout << message;
}

// Costruttore della classe interfaccia. Crea la directory di log se non esiste e crea e inizializza il file di log.
Interfaccia::Interfaccia(std::string logFileName) {
    if(!directoryExists(logDirName)) createDirectory(logDirName);

    if(logFileName == "-Log") {
        logFileName = getCurrentDateTime(true) + logFileName + ".txt";
        nomeFileLog = logFileName;
    }else{
        nomeFileLog = logFileName;
    }
    nomeFileLog = logDirName+"/"+nomeFileLog;    
    initializeFileLog();
}

// Distruttore della classe interfaccia. 
Interfaccia::~Interfaccia() {
    endFileLog();
}
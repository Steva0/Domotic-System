#include "../include/Interfaccia.h"

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

// Inizializza il file di log data e ora nel momento in cui il programma è stato avviato
void Interfaccia::initializeFileLog() {
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if(logFile.is_open()) {
        logFile << "Programma avviato alle: " << getCurrentDateTime() << "\n";
        logFile.close();
    }
}

// Scrive sul file di log data e ora nel momento in cui il programma è stato terminato
void Interfaccia::endFileLog() {
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if(logFile.is_open()) {
        logFile << "Programma terminato alle: " << getCurrentDateTime() << "\n\n";
        logFile.close();
    }
}

void Interfaccia::showMessage(const std::string& message) {
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if(logFile.is_open()) {
        showMessage(message, std::cout, logFile);
    }else{ 
        showMessage(message, std::cout);
    }
}

void Interfaccia::showMessage(const std::string& message, std::ostream& outputStream) {
    std::string formattedMessage = "[" + convertIntToTime(currentSystemTime) + "] " + message + "\n";
    outputStream << formattedMessage << std::endl;
}

void Interfaccia::showMessage(const std::string& message, std::ostream& outputStream, std::ofstream& fileStream) {
    std::string formattedMessage = "[" + convertIntToTime(currentSystemTime) + "] " + message;
    if(fileStream.is_open()) {
        fileStream << formattedMessage << std::endl;
    }
    outputStream << formattedMessage << std::endl;
}

// Questa funzione permette all'utente di inserire il nome di un dispositivo anche se esso contiene spazi.
// Riceve un vettore contenente tutte le parole dall'input separate da spazi e restituisce un nuovo vettore in cui
// il nome del dispositivo occupa una sola cella pur avendo spazi al suo interno.
std::vector<std::string> parseInputString(const std::vector<std::string> inputArray, const std::string& command) {
    std::vector<std::string> outputArray;
    std::string deviceName = "";
    int inputSize = inputArray.size();
    if (inputSize == 1) {
        return inputArray;
    }

    if ((command == "set" && inputArray.at(1) != "time") || command == "rm" || (command == "show" && inputArray.size() > 1)) {
        int index = 1;
        for (; index < inputSize; index++) {
            if (command == "set" && (isdigit(inputArray.at(index).at(0)) || inputArray.at(index) == "on" || inputArray.at(index) == "off")) {
                break;
            }
            deviceName += inputArray.at(index) + " ";
        }

        deviceName = deviceName.substr(0, deviceName.size() - 1); // Tolgo spazio finale
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
void incompleteOrWrongCommand(std::string command, bool recursive=false) {
    std::string exception = "";
    if(!recursive){
        exception += "Comando incompleto o non valido!\n";
    }
    if(command == "set time") {
        exception += "Sintassi corretta: set time $TIME\n";
    }
    if(command == "set device") {
        exception += "Sintassi corretta: set $DEVICE_NAME on/off\n";
        exception += "                   set $DEVICE_NAME ${START} [${STOP}]";
    }
    if(command == "rm") {
        exception += "Sintassi corretta: rm $DEVICE_NAME";
    }
    if(command == "reset") {
        exception += "Sintassi corretta: reset time/timers/all";
    }
    if(command == "set") {
        exception += "Sintassi corretta: set time $TIME\n";
        exception += "Sintassi corretta: set $DEVICE_NAME on/off\n";
        exception += "                   set $DEVICE_NAME ${START} [${STOP}]";
    }
    if(command == "fullCommands") {
        exception += "Comandi disponibili:\n";
        exception += "  set time $TIME\n";
        exception += "  set $DEVICE_NAME on/off\n";
        exception += "  set $DEVICE_NAME ${START} [${STOP}]\n";
        exception += "  rm $DEVICE_NAME\n";
        exception += "  show [$DEVICE_NAME]\n";
        exception += "  reset time/timers/all";
        throw infoError(exception);
    }
    throw std::invalid_argument(exception);
}

// Controlla che il tempo inserito sia nel formato corretto mm:ss
bool checkWrongTimeFormat(std::string timeType, int time) {
    if(time == -1) {
        throw std::invalid_argument("Formato orario [" + timeType + "] non valido! (mm:ss)");
        return true;
    }
    return false;
}

// Incrementa il consumo e la produzione totale del sistema
void Interfaccia::updateEnergyUsage() {
    totalProduced += dispositiviAccesi.producedEnergy();
    totalConsumed += dispositiviAccesi.consumedEnergy();
}

// Funzione che mette nella lista "dispositiviAccesi" un dispositivo e stampa a schermo e sul file che esso si è acceso
void Interfaccia::turnOnDevice(Dispositivo dispositivo) {
    dispositiviAccesi.insert(dispositivo);
    showMessage("Il dispositivo " + dispositivo.getNome() + " si e' acceso.");
    checkKilowatt();
}

// Funzione che mette nella lista "dispositiviSpenti" un dispositivo e stampa a schermo e sul file che esso si è spento
void Interfaccia::turnOffDevice(Dispositivo dispositivo, bool print=true) {
    dispositiviSpenti.insert(dispositivo);
    if(print) {
        showMessage("Il dispositivo " + dispositivo.getNome() + " si e' spento.");
    }
}

// Controlla se esistono dispositivi da accendere e in tal caso li accende spostandoli dalla lista dei programmati in quella degli accesi
void Interfaccia::checkTurnOnDevices() {
    try{        
        std::vector<Dispositivo> dispositiviTempAccesi = dispositiviProgrammati.removeDevicesToPowerOn(currentSystemTime);
        for(Dispositivo dispositivo : dispositiviTempAccesi) {
            turnOnDevice(dispositivo);
        }
    }catch(const std::exception& e) {}
}

// Controlla se esistono dispositivi da spegnere e in tal caso li spegne spostandoli dalla lista degli accesi in quella degli spenti
void Interfaccia::checkTurnOffDevices() {
    try{
        std::vector<Dispositivo> dispositiviTempSpenti = dispositiviAccesi.removeDevicesToPowerOff(currentSystemTime);
        for(Dispositivo dispositivo : dispositiviTempSpenti) {
            turnOffDevice(dispositivo);
        }
    }catch(const std::exception& e) {}
}

// Controlla che il consumo totale attuale (consumata - prodotta) sia inferiore al numero di kilowatt disponibili. 
// In caso negativo spegne il primo dispositivo che non sia un generatore e che non sia un dispositivo che deve essere sempre acceso fino a quando il consumo attuale diventa < dei kilowatt disponibili
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

// Imposta un timer al dispositivo che viene passato tramite argomento e stampa l'orario di accensione e spegnimento
void Interfaccia::setDeviceTimer(Dispositivo& dispositivo, int startTime, int endTime, bool alreadySet) {
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

// Cambia lo stato di un dispositivo identificato da un nome, se il dispositivo non è mai stato acceso o programmato viene creato un nuovo dispositivo
// altrimenti il dispositivo viene tolto dalla lista in cui è attualmente e viene modificato il tempo di accensione (e di spegnimento nel caso di un dispositivo cp).
// Infine il dispositivo viene spostato nella lista corretta (accesi o spenti)
void Interfaccia::changeDeviceStatus(std::string newStatus, std::string nomeDispositivo) {
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

// Gestisce il caso in cui si voglia modificare il timer di un dispositivo che ha già un timer chiedendo all'utente cosa vuole fare:
// creare un nuovo dispositivo dello stesso tipo ma con nome diverso avente come timer quello scelto dall'utente
// oppure sovrascrivere il timer del dispositivo con il timer scelto dall'utente spegnendolo se necessario
void Interfaccia::handleDeviceHasAlreadyTimer(std::string nomeDispositivo, int startTime, int endTime) {
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

// Imposta il timer ad un dispositivo gestendo i vari casi:
// se il dispositivo non è mai stato usato, allora lo crea
// se il dispositivo non ha un timer ed è spento allora imposta il timer
// se il dispositivo è acceso o ha già un timer impostato viene lanciata la funzione handleDeviceHasAlreadyTimer() che chiede all'utente cosa vuole fare
void Interfaccia::commandSetDeviceTimer(int startTime, int endTime, std::string nomeDispositivo) {
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

// Controlla la sintassi e la gestione del comando generale SET
// Decide se l'utente vuole cambiare il tempo usando SET TIME oppure vuole impostare un timer o accendere un dispositivo con SET DEVICE
int Interfaccia::handleCommandSet(const std::vector<std::string> &v)
{
    if(v.size() < 2) {
        incompleteOrWrongCommand("set");
        return 1;
    }
    std::string arg = v.at(1);
    if(arg == "time") {
        return handleCommandSetTime(v);
    }else{ 
        return handleCommandSetDevice(v);
    }
}

// Controlla la sintassi e la gestione del comando SET DEVICE con il quale l'utente può
// Accendere o spegnere un dispositivo con SET DEVICE ON/OFF
// Impostare un timer di un dispositivo con SET DEVICE START [STOP], in questo caso controlla anche la validità dei tempi inseriti
int Interfaccia::handleCommandSetDevice(const std::vector<std::string> &v) {

    if(v.size() < 3) {
        incompleteOrWrongCommand("set device");
        return 1;
    }
    std::string nomeDispositivo = fixDeviceName(v.at(1));
    std::string arg2 = v.at(2);

    if(arg2 == "on" || arg2 == "off") { // accendo o spengo il dispositivo
        changeDeviceStatus(arg2, nomeDispositivo);
    }else{ // imposto il timer di un dispositivo
        int startTime = -1;
        try{
            startTime = convertTimeToInt(arg2);
        }catch(const std::exception &e) {
            incompleteOrWrongCommand("set device");
            return 1;
        }

        if(checkWrongTimeFormat("startTime", startTime)) {
            return 1;
        }

        int endTime = -1;
        if(v.size() == 4) { // controllo se l'utente ha inserito un tempo di spegnimento
            try{
                endTime = convertTimeToInt(v.at(3));
            }catch(const std::exception &e) {
                incompleteOrWrongCommand("set device");
                return 1;
            }
            if(checkWrongTimeFormat("endTime", endTime)) {
                return 1;
            }
        }
        if(currentSystemTime > startTime) {
            throw std::invalid_argument("Non puoi programmare un dispositivo per il passato!");
        }
        if(endTime <= startTime && endTime != -1){
            throw std::invalid_argument("Il tempo di spegnimento non puo' essere minore o uguale al tempo di accensione.");
        }
        commandSetDeviceTimer(startTime, endTime, nomeDispositivo);
    }
    return 0;
}
// Controlla la sintassi e la gestione del comando SET TIME e verifica la correttezza dell'orario inserito
// Fa scorrere il tempo minuto per minuto fino al tempo voluto e ogni minuto effettua le seguenti operazioni:
//  -Aumenta il tempo di accensione di ogni dispositivo
//  -Aggiorna il consumo e la produzione di energia totale
//  -Controlla se ci sono dispositivi da spegnere
//  -Controlla se ci sono dispositivi da accendere
//  -Controlla che il consumo totale non superi il numero di kilowatt disponibili
int Interfaccia::handleCommandSetTime(const std::vector<std::string> &v) {
    if(v.size() < 3) {
        incompleteOrWrongCommand("set time");
        return 1;
    }

    int wantedTime = convertTimeToInt(v.at(2));

    if(checkWrongTimeFormat("wantedTime", wantedTime)) {
        return 1;
    }

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
int Interfaccia::handleCommandRm(const std::vector<std::string> &v) {
    if(v.size() < 2) {
        incompleteOrWrongCommand("rm");
        return 1;
    }

    std::string nomeDispositivo = fixDeviceName(v.at(1));
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
int Interfaccia::handleCommandShow(const std::vector<std::string> &v) {
    if(v.size() < 2) {
        std::ostringstream message;
        message << std::fixed << std::setprecision(3);

        message << "Attualmente il sistema ha prodotto " << totalProduced << "kWh e ha consumato " << totalConsumed << "kWh. Nello specifico:\n\t";
        if(dispositiviAccesi.isEmpty() && dispositiviProgrammati.isEmpty() && dispositiviSpenti.isEmpty()) {
            message << "Il sistema non sta gestendo alcun dispositivo.";
        }else{
            message << dispositiviAccesi.showAll();
            if(!dispositiviProgrammati.isEmpty() && !dispositiviAccesi.isEmpty()) {
                message << "\n\t";
            }
            message << dispositiviProgrammati.showAll(currentSystemTime);
            if(!dispositiviSpenti.isEmpty() && !dispositiviProgrammati.isEmpty()) {
                message << "\n\t";
            }
            message << dispositiviSpenti.showAll();
        }
        showMessage(message.str());
    }else if(v.size() == 2) {
        std::string argNome = v.at(1);
        if(argNome == "debug") {
            std::string message;
            message += "\nACCESI\n" + dispositiviAccesi.showAllDebug();
            message += "\nPROGRAMMATI\n" + dispositiviProgrammati.showAllDebug();
            message += "\nSPENTI\n" + dispositiviSpenti.showAllDebug();
            showMessage(message);
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

// Controlla la sintassi e la gestione del comando generale RESET e dei 3 possibili casi:
//  -RESET TIME
//  -RESET TIMERS
//  -RESET ALL
int Interfaccia::handleCommandReset(const std::vector<std::string> &v) {
    if(v.size() < 2) {
        incompleteOrWrongCommand("reset");
        return 1;
    }
    std::string arg = v.at(1);
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

// Controlla la sintassi del comando inserito dall'utente e in base al comando inserito lancia la funzione per gestire correttamente il comando 
int Interfaccia::parseAndRunCommand(std::string userInput) {

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
        incompleteOrWrongCommand("fullCommands");
        return 1;
    }

    if(command == "help") {
        incompleteOrWrongCommand("fullCommands", true);
        return 1;
    }
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
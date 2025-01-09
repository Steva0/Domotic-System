#include "../include/Interfaccia.h"

bool createDirectory(const std::string& folderName) {
#if defined(_WIN32) || defined(_WIN64)
    std::string command = "mkdir \"" + folderName + "\"";
#else
    std::string command = "mkdir -p \"" + folderName + "\"";
#endif
    return std::system(command.c_str()) == 0;
}

bool directoryExists(const std::string& dirName) {
    struct stat info;
    return (stat(dirName.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

Interfaccia::Interfaccia(std::string logFileName) {

    if(!directoryExists(logDirName)) createDirectory(logDirName);

    if(logFileName == "-Log"){
        logFileName = getCurrentDateTime(true) + logFileName + ".txt";
        nomeFileLog = logFileName;
    }else{
        nomeFileLog = logFileName;
    }

    nomeFileLog = logDirName+"/"+nomeFileLog;
    
    initializeFileLog();
}

Interfaccia::~Interfaccia() {
    endFileLog();
}

//Converte il tempo in formato hh:mm in minuti
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
    } else {
        int hours = std::stoi(time);
        if(hours < 0 || hours > 23) {
            return -1;
        }
        return hours * 60;
    }
}

std::string convertIntToTime(int minuti) {
    int ore = minuti / 60 % 24;
    int min = minuti % 60;
    return (ore < 10 ? "0" : "") + std::to_string(ore) + ":" + (min < 10 ? "0" : "") + std::to_string(min);
}

std::string Interfaccia::getCurrentDateTime(bool fileNameCreation) const {
    // Ottieni l'orario corrente
    std::time_t now = std::time(nullptr);
    
    // Convertilo in una struttura tm
    std::tm* localTime = std::localtime(&now);

    // Formatta data e ora
    char buffer[100];
    if(fileNameCreation){
        std::strftime(buffer, sizeof(buffer), "%d-%m-%y %H_%M_%S", localTime);
        return buffer;
    }
    std::strftime(buffer, sizeof(buffer), "%d-%m-%y %H:%M:%S", localTime);

    // Restituisci il risultato come std::string
    return buffer;
}

void Interfaccia::initializeFileLog(){
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if (logFile.is_open()) {
        logFile << "Programma avviato alle: " << getCurrentDateTime() << "\n";
        logFile.close();
    }
}

void Interfaccia::endFileLog(){
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if (logFile.is_open()) {
        logFile << "Programma terminato alle: " << getCurrentDateTime() << "\n\n";
        logFile.close();
    }
}

void Interfaccia::showMessage(const std::string& message){
    std::ofstream logFile(nomeFileLog, std::ios::app);
    if (logFile.is_open()) {
        showMessage(message, std::cout, logFile);
    } else {
        showMessage(message, std::cout);
    }
}

void Interfaccia::showMessage(const std::string& message, std::ostream& outputStream) {
    std::string formattedMessage = "[" + convertIntToTime(currentSystemTime) + "] " + message + "\n";
    outputStream << formattedMessage << std::endl;
}

void Interfaccia::showMessage(const std::string& message, std::ostream& outputStream, std::ofstream& fileStream) {
    std::string formattedMessage = "[" + convertIntToTime(currentSystemTime) + "] " + message;
    if (fileStream.is_open()) {
        fileStream << formattedMessage << std::endl;
    }
    outputStream << formattedMessage << std::endl;
}

std::vector<std::string> parseInputString(const std::vector<std::string> inputArray, const std::string& command) {
    std::vector<std::string> outputArray;
    std::string deviceName = "";
    int inputSize = inputArray.size();
    int index;

    if(inputSize == 1) {
        return inputArray;
    }

    if(command == "set" && inputArray.at(1) != "time") {
        //scorro fino a che non trovo on/off o orario
        for (int i = 1; i < inputSize; i++) {
            if (isdigit(inputArray.at(i).at(0)) || inputArray.at(i) == "on" || inputArray.at(i) == "off") {
                index = i;
                break;
            }
            deviceName += inputArray.at(i) + " ";
        }

        deviceName = deviceName.substr(0, deviceName.size() - 1); // Tolgo spazio finale
        outputArray.push_back(command);
        outputArray.push_back(deviceName);

        if(inputArray.at(1) != "on" && inputArray.at(1) != "off") {
            for(int i = index; i < inputSize; i++) {
                outputArray.push_back(inputArray.at(i));
            }
        }else{
            outputArray.push_back(inputArray.at(index));
        }
        
    }else if(command == "rm" || (command == "show" && inputArray.size()>1)) {
        if (inputArray.size() == 2) return inputArray;
        //scorro tutta la stringa fino alla fine e carico in nomeDispositivo
        
        for(int i = 1; i < inputSize; i++) {
            deviceName += inputArray.at(i) + " ";
        }

        deviceName = deviceName.substr(0, deviceName.size() - 1); // Tolgo spazio finale
        outputArray.push_back(command);
        outputArray.push_back(deviceName);
    } else {
        return inputArray;
    }
    return outputArray;
}

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

void incompleteOrWrongCommand(std::string command, bool recursive=false) {
    std::string exception = "";
    if(!recursive) exception += "Comando incompleto o non valido!\n";
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

bool checkWrongTimeFormat(std::string timeType, int time) {
    if(time == -1) {
        throw std::invalid_argument("Formato orario [" + timeType + "] non valido! (mm:ss)");
        return true;
    }
    return false;
}

void Interfaccia::updateConsumo() {
    totalProduced += dispositiviAccesi.getProdotta()/60.0;
    totalUsed += dispositiviAccesi.getUsata()/60.0;
}

void Interfaccia::updateActiveTime() {
    dispositiviAccesi.incrementTimeOn();
}

void Interfaccia::turnOnDevice(Dispositivo dispositivo, int currentTime) {
    dispositiviAccesi.insert(dispositivo);
    showMessage("Il dispositivo " + dispositivo.getNome() + " si e' acceso.");
    checkKilowatt(currentTime);
}

void Interfaccia::turnOffDevice(Dispositivo dispositivo, int currentTime, bool print=true) {
    dispositiviSpenti.insert(dispositivo);
    if(print) {
        showMessage("Il dispositivo " + dispositivo.getNome() + " si e' spento.");
    }
}

void Interfaccia::checkTurnOnDevices(int currentTime) {
    try{        
        std::vector<Dispositivo> dispositiviTempAccesi = dispositiviProgrammati.turnOnDevices(currentTime);
        for(Dispositivo dispositivo : dispositiviTempAccesi) {
            turnOnDevice(dispositivo, currentTime);
        }
    }catch(const std::exception& e) {}
}

void Interfaccia::checkTurnOffDevices(int currentTime) {
    try{
        std::vector<Dispositivo> dispositiviTempSpenti = dispositiviAccesi.removeAllDispositiviOff(currentTime);
        for(Dispositivo dispositivo : dispositiviTempSpenti) {
            turnOffDevice(dispositivo, currentTime);
        }
    }catch(const std::exception& e) {}
}

void Interfaccia::checkKilowatt(int currentTime) {
    bool removed = false;
    std::vector<std::string> dispositiviSpentiString;

    while(dispositiviAccesi.getConsumoAttuale(currentTime) + MAX_KILOWATT < 0) {// se ho superato i kilowatt tolgo il primo dispositivo che non sia sempre acceso
        if(!removed) {
            showMessage("E' stato superato il limite di kilowatt.");
            removed = true;
        }       
        Dispositivo disp = dispositiviAccesi.removeFirst();
        disp.setOrarioSpegnimento(currentTime);
        turnOffDevice(disp, currentTime, false);
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
        std::cout << "Consumo attuale: " << std::to_string(dispositiviAccesi.getConsumoAttuale(currentTime)) << "kWh." << std::endl;
    }
}

void Interfaccia::changeDeviceStatus(std::string newStatus, std::string nomeDispositivo,int currentTime) {
    //set device status
    if (newStatus == "on") {
        //accendo il dispositivo
        if(dispositiviAccesi.contains(nomeDispositivo)) {
            std::cout << "Dispositivo gia' acceso!";
            std::string risposta;

            bool rispostaOk = false;
            do{
                std::cout << " Vuoi creare un nuovo dispositivo? [y/n] ";
                std::getline(std::cin, risposta);
                if(risposta == "n" || risposta == "N" || risposta == "no") {
                    rispostaOk = true;  //non faccio nulla
                }else if (risposta == "y" || risposta == "Y" || risposta == "yes") {
                    rispostaOk = true;
                    Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, currentTime);
                    turnOnDevice(*dispositivo, currentTime);
                }else{
                    std::cout << "Risposta non valida, riprova" << std::endl;
                }
            }while(!rispostaOk);

        }else{
            if(dispositiviSpenti.contains(nomeDispositivo)) {
                Dispositivo dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
                if (dispositivo.isManual()) {
                    dispositivo.setTimerOff();
                    dispositivo.setOrarioAccensione(currentTime);
                } else {
                    dispositivo.setOrarioAccensione(currentTime);
                }                
                turnOnDevice(dispositivo, currentTime);
            }else{
                Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, currentTime);
                turnOnDevice(*dispositivo, currentTime);
            }
        }
    }else if (newStatus == "off") {
        //spengo il dispositivo e sposto su array dispositivi spenti

        if(dispositiviAccesi.contains(nomeDispositivo)) {
            Dispositivo dispositivo = dispositiviAccesi.removeDispositivoName(nomeDispositivo);

            dispositivo.setOrarioSpegnimento(currentTime);

            turnOffDevice(dispositivo, currentTime);
        }else{
            if(dispositiviProgrammati.contains(nomeDispositivo)) {
                throw std::invalid_argument("Il dispositivo si deve ancora accendere! Per rimuovere il timer usare rm.");
            }
            if(dispositiviSpenti.contains(nomeDispositivo)) {
                throw std::invalid_argument("Dispositivo gia' spento!");
            }
            throw std::invalid_argument("Dispositivo inesistente!");
        }
    }
}

void Interfaccia::setDeviceTimer(Dispositivo& dispositivo, int startTime, int endTime) {
    if(dispositivo.isCP()) {
        dispositivo.setOrarioAccensione(startTime);
    }else{
        dispositivo.setOrarioSpegnimento(endTime);
        dispositivo.setOrarioAccensione(startTime);
    }    
}

void Interfaccia::handleDeviceHasAlreadyTimer(std::string nomeDispositivo, int startTime, int endTime, int currentTime) {
    //chiedo all'utente se voglio cambiare il timer o crearne uno nuovo però con un nuovo dispositivo con un altro numero seriale
    std::cout << "Il dispositivo " << nomeDispositivo << " ha gia' un timer!" << std::endl;
    bool rispostaOk = false;
    std::string risposta;
    do{
        std::cout << "Vuoi sovrascrivere il timer? [y/n] ";
        std::getline(std::cin, risposta);
        if(risposta == "n" || risposta == "N" || risposta == "no") {
            //creo un nuovo dispositivo 
            rispostaOk = true;
            Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime, true);
            if(currentTime == startTime) {
                turnOnDevice(*dispositivo, currentTime);
            }else{
                dispositiviProgrammati.insert(*dispositivo);
            }

        }else if (risposta == "y" || risposta == "Y" || risposta == "yes") {
            //sovrascrivo il timer
            //se il dispositivo ha un tempo di accensione ma è ancora spento, allora sovrascrivo i tempi di accensione e spegnimento
            //se il dispositivo ha un tempo di accensione ma è già acceso allora spegno il dispositivo e lo riaccendo con i nuovi tempi
            rispostaOk = true;
            bool daAccendere = false;

            Dispositivo dispositivo;
            Dispositivo dispositivoBk;

            if(dispositiviProgrammati.contains(nomeDispositivo)) {//il dispositivo deve ancora accendersi
                dispositivo = dispositiviProgrammati.removeDispositivoName(nomeDispositivo);
                dispositivoBk = dispositivo;
                daAccendere = true;

            }else if(dispositiviAccesi.contains(nomeDispositivo)) {//il dispositivo è già acceso
                dispositivo = dispositiviAccesi.removeDispositivoName(nomeDispositivo);
                dispositivoBk = dispositivo;
                dispositivo.setOrarioSpegnimento(Dispositivo::MAX_MINUTI_GIORNATA);
            }

            try{
                setDeviceTimer(dispositivo, startTime, endTime);
                if(currentTime == startTime) {
                    turnOnDevice(dispositivo, currentTime);
                }else{
                    dispositiviProgrammati.insert(dispositivo);
                }
            }catch(const std::exception& e) {    //rollback
                std::cout << e.what() << std::endl; 
                if(daAccendere) {
                    dispositiviProgrammati.insert(dispositivoBk);
                }else{
                    dispositiviAccesi.insert(dispositivoBk);
                }
            }

        }else{
            std::cout << "Risposta non valida, riprova" << std::endl;
        }

    }while(!rispostaOk);
}

void Interfaccia::commandSetDeviceTimer(int startTime, int endTime, std::string nomeDispositivo, int currentTime) {
    //cerco se esiste gia il dispositivo
    if(currentTime > startTime) {
        throw std::invalid_argument("Non puoi programmare un dispositivo per il passato!");
    }

    if(dispositiviAccesi.contains(nomeDispositivo) || dispositiviProgrammati.contains(nomeDispositivo)) {
        handleDeviceHasAlreadyTimer(nomeDispositivo, startTime, endTime, currentTime);

    }else if(dispositiviSpenti.contains(nomeDispositivo)) {

        Dispositivo dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
        Dispositivo dispositivoBk = dispositivo;
        
        try{
            setDeviceTimer(dispositivo, startTime, endTime);
            dispositivo.setHasTimer(true);
            if(currentTime == startTime) {
                turnOnDevice(dispositivo, currentTime);
            }else{
                dispositiviProgrammati.insert(dispositivo);
            }
        }catch(const std::exception& e) { //rollback
            dispositiviSpenti.insert(dispositivoBk);
        }
    }else{
        //se non esiste lo creo con CreaDispositivo::creaDispositivo
        Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime, true);

        if(currentTime == startTime) {
            turnOnDevice(*dispositivo, currentTime);
        }else{
            dispositiviProgrammati.insert(*dispositivo);
        }
    }
}

std::vector<Dispositivo> operator+(std::vector<Dispositivo>&& array1, std::vector<Dispositivo>&& array2) {
    // Crea un nuovo vettore per contenere entrambi i vettori
    std::vector<Dispositivo> result = array1;

    // Aggiunge tutti gli elementi di array2 a result
    result.insert(result.end(), array2.begin(), array2.end());

    return result;
}

int Interfaccia::handleCommandSet(std::vector<std::string> &v)
{
    if (v.size() < 2)
    {
        incompleteOrWrongCommand("set");
        return 1;
    }
    std::string arg = v.at(1);
    if (arg == "time")
    {
        return handleCommandSetTime(v);
    }
    else
    {
        return handleCommandSetDevice(v);
    }
}

int Interfaccia::handleCommandSetDevice(const std::vector<std::string> &v)
{

    if (v.size() < 3)
    {
        incompleteOrWrongCommand("set device");
        return 1;
    }
    std::string nomeDispositivo = fixDeviceName(v.at(1));
    std::string arg2 = v.at(2);

    if (arg2 == "on" || arg2 == "off")
    {
        // accendo o spengo il dispositivo
        changeDeviceStatus(arg2, nomeDispositivo, currentSystemTime);
    }
    else
    { // set device timer
        int startTime = -1;

        try
        {
            startTime = convertTimeToInt(arg2);
        }
        catch (const std::exception &e)
        {
            incompleteOrWrongCommand("set device");
            return 1;
        }

        if (checkWrongTimeFormat("startTime", startTime))
            return 1;

        int endTime = -1;
        if (v.size() == 4)
        { // controllo se l'utente ha inserito un tempo di spegnimento
            try
            {
                endTime = convertTimeToInt(v.at(3));
            }
            catch (const std::exception &e)
            {
                incompleteOrWrongCommand("set device");
                return 1;
            }
            if (checkWrongTimeFormat("endTime", endTime))
                return 1;
        }
        commandSetDeviceTimer(startTime, endTime, nomeDispositivo, currentSystemTime);
    }
    return 0;
}

int Interfaccia::handleCommandSetTime(const std::vector<std::string> &v)
{
    if (v.size() < 3) {
        incompleteOrWrongCommand("set time");
        return 1;
    }

    int wantedTime = convertTimeToInt(v.at(2));

    if (checkWrongTimeFormat("wantedTime", wantedTime)) {
        return 1;
    }

    if (currentSystemTime > wantedTime) {
        throw std::invalid_argument("Non puoi tornare indietro nel tempo!");
    }

    // Cambiare tempo usando set time, minuto per minuto usando un ciclo while, controllo tempo spegnimento, accensione, controllo i kilowatt,
    showMessage("L'orario attuale e' " + convertIntToTime(currentSystemTime));
    while (currentSystemTime < wantedTime) {
        ++currentSystemTime;
        // aggiorno il tempo di accensione, aumento consumo, controllo se ci sono dispositivi da accendere
        if (!dispositiviAccesi.isEmpty()) {
            updateActiveTime();
            updateConsumo();
            checkTurnOffDevices(currentSystemTime);
        }

        // controllo se ci sono dispositivi da spegnere
        if (!dispositiviProgrammati.isEmpty())
        {
            checkTurnOnDevices(currentSystemTime);
        }

        // controllo di non aver superato i kilowatt
        if (!dispositiviAccesi.isEmpty())
        {
            checkKilowatt(currentSystemTime);
        }
    }
    showMessage("L'orario attuale e' " + convertIntToTime(currentSystemTime));
    return 0;
}

int Interfaccia::handleCommandRm(const std::vector<std::string> &v)
{
    if (v.size() < 2)
    {
        incompleteOrWrongCommand("rm");
        return 1;
    }

    std::string nomeDispositivo = fixDeviceName(v.at(1));
    if (dispositiviAccesi.contains(nomeDispositivo))
    {
        dispositiviAccesi.removeTimer(nomeDispositivo, currentSystemTime);
    }
    else if (dispositiviProgrammati.contains(nomeDispositivo))
    {
        dispositiviProgrammati.removeTimer(nomeDispositivo, currentSystemTime);
        Dispositivo dispositivo = dispositiviProgrammati.removeDispositivoName(nomeDispositivo);
        dispositiviSpenti.insert(dispositivo);
    }
    else if (dispositiviSpenti.contains(nomeDispositivo))
    {
        std::invalid_argument("Dispositivo gia' spento!");
    }
    else
    {
        std::invalid_argument("Dispositivo inesistente!");
    }
    return 0;
}

int Interfaccia::handleCommandShow(const std::vector<std::string> &v) {
    //mostro tutti i dispositivi (attivi e non) con produzione/consumo di ciascuno dalle 00:00 fino a quando ho inviato il comando show
    //inoltre mostro produzione/consumo totale del sistema dalle 00:00 a quando ho inviato il comando show
    if (v.size() < 2)
    {
        std::ostringstream message;
        message << std::fixed << std::setprecision(3);

        message << "Attualmente il sistema ha prodotto " << totalProduced << "kWh e ha consumato " << totalUsed << "kWh\n\t";
        if(dispositiviAccesi.isEmpty() && dispositiviProgrammati.isEmpty() && dispositiviSpenti.isEmpty()){
            message << "Il sistema non sta gestendo alcun dispositivo.";
        }else{
            message << dispositiviAccesi.showAll();
            message << dispositiviProgrammati.showAll(currentSystemTime);
            message << dispositiviSpenti.showAll();
        }
        showMessage(message.str());
    }
    else if (v.size() == 2)
    {
        std::string argNome = v.at(1);
        if (argNome == "debug")
        {
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

        if (dispositiviAccesi.contains(nomeDispositivo))
        {
            message << std::to_string(std::fabs(dispositiviAccesi.show(nomeDispositivo))) << "kWh.";
            if(!dispositiviProgrammati.isEmpty() || !dispositiviSpenti.isEmpty()) message << "\n\t";
        }
        else if (dispositiviProgrammati.contains(nomeDispositivo))
        {
            message << std::to_string(std::fabs(dispositiviProgrammati.show(nomeDispositivo))) << "kWh.";
            if(!dispositiviSpenti.isEmpty()) message << "\n\t";
        }
        else if (dispositiviSpenti.contains(nomeDispositivo))
        {
            message << std::to_string(std::fabs(dispositiviSpenti.show(nomeDispositivo))) << "kWh.";
        }
        else
        {
            throw std::invalid_argument("Il dispositivo " + nomeDispositivo + " non e' mai stato acceso o programmato.");
        }

        showMessage(message.str());
        
    }
    else
    {
        incompleteOrWrongCommand("show");
        return 1;
    }
    return 0;    
}

int Interfaccia::handleCommandReset(const std::vector<std::string> &v)
{
    if (v.size() < 2)
    {
        incompleteOrWrongCommand("reset");
        return 1;
    }
    std::string arg = v.at(1);
    if (arg == "time")
    {
        // riporto tempo a 00:00, tutti i dispositivi alle condizioni iniziali (?), i timer vengono mantenuti
        currentSystemTime = 0;
        totalProduced = 0;
        totalUsed = 0;

        std::vector<Dispositivo> tempDevices = dispositiviAccesi.removeAllForce() + dispositiviProgrammati.removeAllForce() + dispositiviSpenti.removeAllForce();

        for (Dispositivo dispositivo : tempDevices)
        {
            dispositivo.resetTempoAccensione();
            if (dispositivo.hasTimer())
            {
                dispositiviProgrammati.insert(dispositivo);
            }
            else
            {
                dispositiviSpenti.insert(dispositivo);
            }
        }
    }
    else if (arg == "timers")
    {
        // tolgo tutti i timer impostati e i dispositivi restano nello stato corrente (acceso/spento)
        dispositiviAccesi.resetAllTimers(currentSystemTime);
        std::vector<Dispositivo> removed = dispositiviProgrammati.removeAllForce();
        for (Dispositivo dispositivo : removed)
        {
            dispositiviSpenti.insert(dispositivo);
        }
        dispositiviSpenti.resetAllTimers(currentSystemTime);
    }
    else if (arg == "all")
    {
        // riporto tutto alle condizioni iniziali (orario a 00:00, tolgo tutti i timer, tutti i dispositivi spenti)
        currentSystemTime = 0;
        totalProduced = 0;
        totalUsed = 0;

        std::vector<Dispositivo> tempDevices = dispositiviAccesi.removeAllForce() + dispositiviProgrammati.removeAllForce() + dispositiviSpenti.removeAllForce();

        for (Dispositivo dispositivo : tempDevices)
        {
            dispositivo.resetTempoAccensione();
            dispositiviSpenti.insert(dispositivo);
        }

        dispositiviSpenti.resetAll();
    }
    return 0;
}

int Interfaccia::parseAndRunCommand(std::string userInput) {
    
    if(userInput =="") return 0;
    if(userInput == "fromFile") return 12345;
    if(userInput == "esci" || userInput == "exit" || userInput == "q") return -1;

    std::string s;
    std::stringstream ss(userInput);
    std::vector<std::string> v;

    // divido l'input in argomenti separati da spazi
    while (getline(ss, s, ' ')) {
        v.push_back(s);
    }

    std::string command = v.at(0);

    bool commandOk = false;

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

    if (command == "help") {
        incompleteOrWrongCommand("fullCommands", true);
        return 1;
    }    
    else if (command == "set") {
        return handleCommandSet(v);
    }
    else if (command == "rm") {
        return handleCommandRm(v);
    }
    else if (command == "show") {
        return handleCommandShow(v);
    }
    else if (command == "reset") {
        return handleCommandReset(v);
    }
    return 0;
}
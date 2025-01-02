#include "../include/Interfaccia.h"
#include "../include/RicercaDispositivo.h"
#include "../include/Dispositivo.h"
#include <sstream>
#include <vector>

Interfaccia::Interfaccia(){}//[WIP]

Interfaccia::~Interfaccia(){}//[WIP]


std::vector<std::string> parseInputString(const std::vector<std::string> inputArray, const std::string& command) {
    std::vector<std::string> outputArray;
    std::string deviceName = "";
    int inputSize = inputArray.size();
    int index;

    if(inputSize == 1){
        return inputArray;
    }

    if(command == "set" && inputArray.at(1) != "time"){
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

        if(inputArray.at(1) != "on" && inputArray.at(1) != "off"){
            for(int i = index; i < inputSize; i++){
                outputArray.push_back(inputArray.at(i));
            }
        }else{
            outputArray.push_back(inputArray.at(index));
        }
        
    }else if(command == "rm" || (command == "show" && inputArray.size()>1)){
        if (inputArray.size() == 2){
            return inputArray;
        }
        //scorro tutta la stringa fino alla fine e carico in nomeDispositivo        
        
        for(int i = 1; i < inputSize; i++){
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


void incompleteOrWrongCommand(std::string command, bool recursive=false) {
    std::string exception = "";
    if(!recursive) exception += "Comando incompleto o non valido!\n";
    if(command == "set time"){
        exception += "Sintassi corretta: set time $TIME\n";
    }
    if(command == "set device"){
        exception += "Sintassi corretta: set $DEVICE_NAME on/off\n";
        exception += "                   set $DEVICE_NAME ${START} [${STOP}]";
    }
    if(command == "rm"){
        exception += "Sintassi corretta: rm $DEVICE_NAME";
    }
    if(command == "reset"){
        exception += "Sintassi corretta: reset time/timers/all";
    }
    if(command == "set"){
        exception += "Sintassi corretta: set time $TIME\n";
        exception += "Sintassi corretta: set $DEVICE_NAME on/off\n";
        exception += "                   set $DEVICE_NAME ${START} [${STOP}]";
    }
    if(command == "fullCommands"){
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
    if(time == -1){
        throw std::invalid_argument("Formato orario [" + timeType + "] non valido! (mm:ss)");
        return true;
    }
    return false;
}

void Interfaccia::checkTurnOnDevices(int currentTime, bool& cambiatoQualcosa) {
    try{        
        std::vector<Dispositivo> dispositiviTempAccesi = dispositiviProgrammati.turnOnDevices(currentTime);
        for(Dispositivo disp : dispositiviTempAccesi){
            dispositiviAccesi.insert(disp);
            cambiatoQualcosa = true;    //debug
            std::cout << "Il dispositivo " << disp.getNome() << " e' stato acceso." << std::endl;
        }
                           
    }catch(const std::exception& e){}
}

void Interfaccia::checkTurnOffDevices(int currentTime, bool& cambiatoQualcosa) {
    try{
        std::vector<Dispositivo> dispositiviTempSpenti = dispositiviAccesi.removeAllDispositiviOff(currentTime);
        for(Dispositivo disp : dispositiviTempSpenti){
            disp.incrementaTempoAccensione(currentTime - disp.getOrarioAccensione());
            dispositiviSpenti.insert(disp);
            cambiatoQualcosa = true;    //debug
            std::cout << "Il dispositivo " << disp.getNome() << " e' stato spento." << std::endl;
        }                    
    }catch(const std::exception& e){}
}

void Interfaccia::checkKilowatt(int currentTime, bool& blackout, bool& cambiatoQualcosa) {
    while(dispositiviAccesi.getConsumoAttuale(currentTime) + MAX_KILOWATT < 0){
        // se ho superato i kilowatt tolgo il primo dispositivo che non sia sempre acceso        
        Dispositivo disp = dispositiviAccesi.removeFirst();
        disp.incrementaTempoAccensione(currentTime - disp.getOrarioAccensione());
        disp.setOrarioSpegnimento(currentTime);
        dispositiviSpenti.insert(disp);
        cambiatoQualcosa = true;
        blackout = true;
        std::cout << "Il dispositivo " << disp.getNome() << " e' stato spento causa superamento kilowatt." << std::endl;
    }
}

void Interfaccia::changeDeviceStatus(std::string newStatus, std::string nomeDispositivo,int currentTime){
    //set device status
    if (newStatus == "on"){
        //accendo il dispositivo
        if(dispositiviAccesi.contains(nomeDispositivo)){
            std::cout << "Dispositivo gia' acceso!";
            std::string risposta;

            bool rispostaOk = false;
            do{                
                std::cout << " Vuoi creare un nuovo dispositivo? [y/n] ";
                std::getline(std::cin, risposta);
                std::cout << "Risposta: " << risposta << std::endl; //debug

                if(risposta == "n" || risposta == "N" || risposta == "no"){
                    rispostaOk = true;  //non faccio nulla
                }else if (risposta == "y" || risposta == "Y" || risposta == "yes"){
                    rispostaOk = true;
                    Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, currentTime);
                    dispositiviAccesi.insert(*dispositivo);
                }else{
                    std::cout << "Risposta non valida, riprova" << std::endl;
                }
            }while(!rispostaOk);

        }else{
            if(dispositiviSpenti.contains(nomeDispositivo)){
                std::cout << "Trovato spento [off]" << std::endl; //debug
                Dispositivo dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
                if (dispositivo.isManual()){
                    dispositivo.setTimerOff();
                    std::cout << "Dispositivo manuale, tolgo timer" << std::endl; //debug
                    dispositivo.setOrarioAccensione(currentTime);
                } else {
                    dispositivo.setOrarioSpegnimento(currentTime + dispositivo.getDurataCiclo());
                    dispositivo.setOrarioAccensione(currentTime);
                }                
                dispositiviAccesi.insert(dispositivo);
            }else{
                std::cout << "Non trovato, creo nuovo" << std::endl; //debug
                Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, currentTime);
                dispositiviAccesi.insert(*dispositivo);
            }
        }
    }else if (newStatus == "off"){//set device status
        //spengo il dispositivo e sposto su array dispositivi spenti
        //devo salvare il tempo totale di accensione = currentTime-startTime, 

        if(dispositiviAccesi.contains(nomeDispositivo)){
            std::cout << "Trovato acceso" << std::endl; //debug
            Dispositivo dispositivo = dispositiviAccesi.removeDispositivoName(nomeDispositivo);

            dispositivo.setOrarioSpegnimento(currentTime);

            dispositivo.incrementaTempoAccensione(currentTime - dispositivo.getOrarioAccensione());
            dispositiviSpenti.insert(dispositivo);
        }else{
            if(dispositiviSpenti.contains(nomeDispositivo)){
                throw std::invalid_argument("Dispositivo gia' spento!");
            }
            throw std::invalid_argument("Dispositivo inesistente!");
        }
    }
}

void Interfaccia::setDeviceTimer(Dispositivo& dispositivo, int startTime, int endTime){
    if(dispositivo.isCP()){
        dispositivo.setOrarioAccensione(startTime);
    }else{
        dispositivo.setOrarioSpegnimento(endTime);
        dispositivo.setOrarioAccensione(startTime);
    }    
}

void Interfaccia::handleDeviceHasAlreadyTimer(std::string nomeDispositivo, int startTime, int endTime, int currentTime){
    //chiedo all'utente se voglio cambiare il timer o crearne uno nuovo però con un nuovo dispositivo con un altro nome
    std::cout << "Il dispositivo " << nomeDispositivo << " ha gia' un timer!" << std::endl;
    bool rispostaOk = false;
    std::string risposta;
    do{
        std::cout << "Vuoi sovrascrivere il timer? [y/n] ";
        std::getline(std::cin, risposta);
        std::cout << "Risposta: " << risposta << std::endl; //debug

        if(risposta == "n" || risposta == "N" || risposta == "no"){
            //creo un nuovo dispositivo 
            rispostaOk = true;
            Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime);
            dispositiviProgrammati.insert(*dispositivo);    

        }else if (risposta == "y" || risposta == "Y" || risposta == "yes"){
            //sovrascrivo il timer
            //se il dispositivo ha un tempo di accensione ma è ancora spento (currentTime < tempo di accensione), allora sovrascrivo i tempi di accensione e spegnimento
            //se il dispositivo ha un tempo di accensione ma è già acceso (currentTime > tempo di accensione) allora spegno il dispositivo e lo riaccendo con i nuovi tempi
            rispostaOk = true;
            bool daAccendere = false;
            Dispositivo dispositivo;
            Dispositivo dispositivoBk;
            if(dispositiviProgrammati.contains(nomeDispositivo)){//il dispositivo deve ancora accendersi
                dispositivo = dispositiviProgrammati.removeDispositivoName(nomeDispositivo);
                dispositivoBk = dispositivo;
                daAccendere = true;

            }else if(dispositiviAccesi.contains(nomeDispositivo)){//il dispositivo è già acceso
                dispositivo = dispositiviAccesi.removeDispositivoName(nomeDispositivo);
                dispositivoBk = dispositivo;
                dispositivo.setOrarioSpegnimento(Dispositivo::MAX_MINUTI_GIORNATA);
                dispositivo.incrementaTempoAccensione(currentTime - dispositivo.getOrarioAccensione()); 
            }

            if(dispositivo.isCP()){
                endTime = startTime + dispositivo.getDurataCiclo();
            }
            
            try{
                setDeviceTimer(dispositivo, startTime, endTime);
                if(currentTime == startTime){
                    dispositiviAccesi.insert(dispositivo);
                }else{
                    dispositiviProgrammati.insert(dispositivo);
                }
            }catch(const std::exception& e){
                std::cout << e.what() << std::endl; //debug
                if(daAccendere){
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

void Interfaccia::commandSetDeviceTimer(int startTime, int endTime, std::string nomeDispositivo, int currentTime){
    //cerco se esiste gia il dispositivo    
    if(currentTime > startTime){
        throw std::invalid_argument("Non puoi programmare un dispositivo per il passato!");
    }

    if(dispositiviAccesi.contains(nomeDispositivo) || dispositiviProgrammati.contains(nomeDispositivo)){
        //il dispositivo ha gia' un timer
        handleDeviceHasAlreadyTimer(nomeDispositivo, startTime, endTime, currentTime);

    }else if(dispositiviSpenti.contains(nomeDispositivo)){

        std::cout << "Trovato spento" << std::endl; //debug
        Dispositivo dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
        Dispositivo dispositivoBk = dispositivo;

        if(dispositivo.isCP()){
            endTime = startTime + dispositivo.getDurataCiclo();
        }
        
        try{
            setDeviceTimer(dispositivo, startTime, endTime);
            if(currentTime == startTime){
                dispositiviAccesi.insert(dispositivo);
            }else{
                dispositiviProgrammati.insert(dispositivo);
            }
        }catch(const std::exception& e){
            std::cout << e.what() << std::endl; //debug
            dispositiviSpenti.insert(dispositivoBk);
        }
    }else{
        //se non esiste lo creo con CreaDispositivo::creaDispositivo
        std::cout << "Non trovato, creo nuovo" << std::endl; //debug
        std::cout << startTime << " " << endTime << std::endl; //debug
        Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime);

        if(currentTime == startTime){
            dispositiviAccesi.insert(*dispositivo);
        }else{
            dispositiviProgrammati.insert(*dispositivo);
        }
    }
}

int Interfaccia::parseAndRunCommand(std::string userInput) {
    
    if(userInput =="") return 1;
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

    for(std::string s:possibleCommands){
        if(s == command){
            commandOk = true;
            break;
        }
    }

    v = parseInputString(v, command);

    if(!commandOk){
        incompleteOrWrongCommand("fullCommands");
        return 1;
    }

    if (command == "help")
    {
        incompleteOrWrongCommand("fullCommands", true);
    }
    
    if (command == "set") {     
        if(v.size() < 2){
            incompleteOrWrongCommand("set");
            return 1;
        }
        std::string arg = v.at(1);
        if (arg == "time") {
            
            if(v.size() < 3){
                incompleteOrWrongCommand("set time");
                return 1;
            }

            int wantedTime = convertTimeToInt(v.at(2));

            if (checkWrongTimeFormat("wantedTime", wantedTime)) return 1;

            if(currentTime > wantedTime){
                throw std::invalid_argument("Non puoi tornare indietro nel tempo!");
            }

            // Cambiare tempo usando set time, minuto per minuto usando un ciclo while, controllo tempo spegnimento, accensione, controllo i kilowatt,

            while(currentTime < wantedTime){
                bool cambiatoQualcosa = false;   //debug
                bool blackout = false;

                //controllo se ci sono dispositivi da accendere
                if(!dispositiviAccesi.isEmpty()){
                   checkTurnOffDevices(currentTime, cambiatoQualcosa);
                }

                //controllo se ci sono dispositivi da spegnere
                if(!dispositiviProgrammati.isEmpty()){
                    checkTurnOnDevices(currentTime, cambiatoQualcosa);
                }
                
                //controllo di non aver superato i kilowatt    
                if(!dispositiviAccesi.isEmpty()){
                    checkKilowatt(currentTime, blackout, cambiatoQualcosa);
                }                              

                if (cambiatoQualcosa)
                {
                    std::cout << "\nTime: " << convertIntToTime(currentTime) << std::endl; //debug
                    if (blackout){
                        std::cout << "Superato il limite di kilowatt!" << std::endl;
                    }

                    std::cout<< "Consumo attuale: " << std::to_string(dispositiviAccesi.getConsumoAttuale(currentTime))<<std::endl; //debug
                    
                    std::cout << "\nACCESI " << dispositiviAccesi.showAll() << std::endl; //debug
                    std::cout << "\nPROGRAMMATI " << dispositiviProgrammati.showAll() << std::endl; //debug
                    std::cout << "\nSPENTI " << dispositiviSpenti.showAll() << std::endl; //debug
                    
                    cambiatoQualcosa = false;
                }

                currentTime++;
            }
            
        }else{
            if(v.size() < 3){
                incompleteOrWrongCommand("set device");
                return 1;
            }
            std::string nomeDispositivo = RicercaDispositivo::ricercaDispositivoSimile(arg, dispositiviPredefiniti);
            std::string arg2 = v.at(2);
            
            if (arg2 == "on" || arg2 == "off") {
                //accendo o spengo il dispositivo
                changeDeviceStatus(arg2, nomeDispositivo, currentTime);

            }else{ //set device timer
                int startTime = -1;

                try{
                    startTime = convertTimeToInt(arg2);
                }catch (const std::exception& e){
                    incompleteOrWrongCommand("set device");
                    return 1;
                }
                
                if(checkWrongTimeFormat("startTime", startTime)) return 1;

                int endTime = -1;
                if(v.size() == 4){//controllo se l'utente ha inserito un tempo di spegnimento
                    try{
                        endTime = convertTimeToInt(v.at(3));
                    }catch (const std::exception& e){
                        incompleteOrWrongCommand("set device");
                        return 1;
                    }
                    if(checkWrongTimeFormat("endTime", endTime)) return 1;
                }
                commandSetDeviceTimer(startTime, endTime, nomeDispositivo, currentTime);
            }
        }
    }
    else if (command == "rm") {
        if(v.size() < 2){
            incompleteOrWrongCommand("rm");
            return 1;
        }

        v = parseInputString(v, command);

        std::string nomeDispositivo = RicercaDispositivo::ricercaDispositivoSimile(v.at(1), dispositiviPredefiniti);
        if(dispositiviAccesi.contains(nomeDispositivo)) {
            dispositiviAccesi.removeTimer(nomeDispositivo, currentTime);
        }
        else if (dispositiviProgrammati.contains(nomeDispositivo)) {
            dispositiviProgrammati.removeTimer(nomeDispositivo, currentTime);
            Dispositivo dispositivo = dispositiviProgrammati.removeDispositivoName(nomeDispositivo);
            dispositiviSpenti.insert(dispositivo);
        }
        else if (dispositiviSpenti.contains(nomeDispositivo)) {
            std::invalid_argument("Dispositivo gia' spento!");
        }
        else {
            std::invalid_argument("Dispositivo inesistente!");
        }
    }
    else if (command == "show"){//[WIP]
        //mostro tutti i dispositivi (attivi e non) con produzione/consumo di ciascuno dalle 00:00 fino a quando ho inviato il comando show
        //inoltre mostro produzione/consumo totale del sistema dalle 00:00 a quando ho inviato il comando show
        if (v.size() < 2){
            std::cout << "ACCESI" << std::endl << dispositiviAccesi.showAll() << std::endl;
            std::cout << "PROGRAMMATI" << std::endl << dispositiviProgrammati.showAll() << std::endl;
            std::cout << "SPENTI" << std::endl << dispositiviSpenti.showAll() << std::endl;
            return 1;
        } else if (v.size() == 2){
            std::string argNome = v.at(1);
            std::string nomeDispositivo = RicercaDispositivo::ricercaDispositivoSimile(argNome, dispositiviPredefiniti);
            std::cout << dispositiviAccesi.show(nomeDispositivo) << std::endl;
        } else{
            incompleteOrWrongCommand("show");
            return 1;
        }        
    }
    else if (command == "reset"){
        if(v.size() < 2){
            incompleteOrWrongCommand("reset");
            return 1;
        }
        std::string arg = v.at(1);
        if (arg == "time") {
            currentTime = 0;

            //riporto tempo a 00:00, tutti i dispositivi alle condizioni iniziali (?), i timer vengono mantenuti
        } else if (arg == "timers") {
            //tolgo tutti i timer impostati e i dispositivi restano nello stato corrente (acceso/spento)
            dispositiviAccesi.resetAllTimers(currentTime);
        } else if (arg == "all") {
            //riporto tutto alle condizioni iniziali (orario a 00:00, tolgo tutti i timer, tutti i dispositivi spenti)
            currentTime = 0;

            std::vector<Dispositivo> dispositiviTempSpenti = dispositiviAccesi.removeAllDispositiviOff(Dispositivo::MAX_MINUTI_GIORNATA);
            for(Dispositivo disp : dispositiviTempSpenti){
                disp.resetTempoAccensione();
                dispositiviSpenti.insert(disp);
            }

            dispositiviSpenti.resetAll();
        }
    }
    return 1;
}

//Converte il tempo in formato hh:mm in minuti
int Interfaccia::convertTimeToInt(std::string time) {    
    std::string s;
    std::stringstream ss(time);
    std::vector<std::string> v;
    bool explicitTime = false;

    for(char c : time){
        if(c == ':'){
            explicitTime = true;
            break;
        }else if(!isdigit(c)){
            return -1;
        }
    }
    
    if(explicitTime){
        while (getline(ss, s, ':')) {
            v.push_back(s);
        }
        int hours = std::stoi(v.at(0));
        int minutes = std::stoi(v.at(1));
        if(hours < 0 || hours > 23 || minutes < 0 || minutes > 59){
            return -1;
        }
        return hours * 60 + minutes;
    }else{
        int hours = std::stoi(time);
        if(hours < 0 || hours > 23){
            return -1;
        }
        return hours * 60;
    }
}

std::string Interfaccia::convertIntToTime(int minuti)  {
    int ore = minuti / 60 % 24;
    int min = minuti % 60;
    return (ore < 10 ? "0" : "") + std::to_string(ore) + ":" + (min < 10 ? "0" : "") + std::to_string(min);
}
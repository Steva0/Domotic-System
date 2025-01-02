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
    if(!recursive) std::cout << "Comando incompleto o non valido!" << std::endl;
    if(command == "set time"){
        std::cout << "Sintassi corretta: set time $TIME " << std::endl;
    }
    if(command == "set device"){
        std::cout << "Sintassi corretta: set $DEVICE_NAME on/off" << std::endl;
        std::cout << "                   set $DEVICE_NAME ${START} [${STOP}]" << std::endl;
    }
    if(command == "rm"){
        std::cout << "Sintassi corretta: rm $DEVICE_NAME" << std::endl;
    }
    if(command == "reset"){
        std::cout << "Sintassi corretta: reset time/timers/all" << std::endl;
    }
    if(command == "set"){
        incompleteOrWrongCommand("set time", true);
        incompleteOrWrongCommand("set device", true);
    }
    if(command == "fullCommands"){
        std::cout << "Comandi disponibili:" << std::endl;
        std::cout << "  set time $TIME" << std::endl;
        std::cout << "  set $DEVICE_NAME on/off" << std::endl;
        std::cout << "  set $DEVICE_NAME ${START} [${STOP}]" << std::endl;
        std::cout << "  rm $DEVICE_NAME" << std::endl;
        std::cout << "  show" << std::endl;
        std::cout << "  reset time/timers/all" << std::endl;
    }
}

bool checkWrongTimeFormat(std::string timeType, int time) {
    if(time == -1){
        std::cout << "Formato orario [" << timeType << "] non valido!" << std::endl;
        return true;
    }
    return false;
}

void Interfaccia::checkTurnOnDevices(int currentTime, bool& cambiatoQualcosa) {
    try{        
        std::vector<Dispositivo> dispositiviTempAccesi = dispositiviDaAccendere.turnOnDevices(currentTime);
        for(Dispositivo disp : dispositiviTempAccesi){
            dispositiviAccesi.insert(disp);
            cambiatoQualcosa = true;    //debug
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
        }                    
    }catch(const std::exception& e){}
}

void Interfaccia::checkKilowatt(int currentTime, bool& blackout, bool& cambiatoQualcosa) {
    while(dispositiviAccesi.getConsumoAttuale(currentTime) + MAX_KILOWATT < 0){
        // se ho superato i kilowatt tolgo il primo dispositivo che non sia sempre acceso        
        Dispositivo disp = dispositiviAccesi.removeFirst();
        dispositiviSpenti.insert(disp);
        cambiatoQualcosa = true;
        blackout = true;
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
                std::cout << "Trovato spento" << std::endl; //debug
                Dispositivo dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
                if (dispositivo.isManual()){
                    dispositivo.setTimerOff();
                    std::cout << "Dispositivo manuale, tolgo timer" << std::endl;
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
            std::cout << "Dispositivo gia' spento!" << std::endl;
        }
    }
}

void Interfaccia::setDeviceTimer(Dispositivo& dispositivo, int startTime, int endTime){
    dispositivo.setOrarioSpegnimento(endTime);
    dispositivo.setOrarioAccensione(startTime);
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
            dispositiviDaAccendere.insert(*dispositivo);    

        }else if (risposta == "y" || risposta == "Y" || risposta == "yes"){
            //sovrascrivo il timer
            //se il dispositivo ha un tempo di accensione ma è ancora spento (currentTime < tempo di accensione), allora sovrascrivo i tempi di accensione e spegnimento
            //se il dispositivo ha un tempo di accensione ma è già acceso (currentTime > tempo di accensione) allora spegno il dispositivo e lo riaccendo con i nuovi tempi
            rispostaOk = true;

            Dispositivo dispositivo;

            if(dispositiviDaAccendere.contains(nomeDispositivo)){//il dispositivo deve ancora accendersi
                dispositivo = dispositiviDaAccendere.removeDispositivoName(nomeDispositivo);

            }else if(dispositiviAccesi.contains(nomeDispositivo)){//il dispositivo è già acceso
                dispositivo = dispositiviAccesi.removeDispositivoName(nomeDispositivo);
                dispositivo.setOrarioSpegnimento(currentTime);
                dispositivo.incrementaTempoAccensione(dispositivo.getOrarioSpegnimento() - dispositivo.getOrarioAccensione()); 
            }

            if(dispositivo.isCP()){
                endTime = startTime + dispositivo.getDurataCiclo();
            }

            setDeviceTimer(dispositivo, startTime, endTime);
            dispositiviDaAccendere.insert(dispositivo);

        }else{
            std::cout << "Risposta non valida, riprova" << std::endl;
        }

    }while(!rispostaOk);
}

void Interfaccia::commandSetDeviceTimer(int startTime, int endTime, std::string nomeDispositivo, int currentTime){
    //cerco se esiste gia il dispositivo    

    if(dispositiviAccesi.contains(nomeDispositivo) || dispositiviDaAccendere.contains(nomeDispositivo)){
        //il dispositivo ha gia' un timer
        handleDeviceHasAlreadyTimer(nomeDispositivo, startTime, endTime, currentTime);

    }else if(dispositiviSpenti.contains(nomeDispositivo)){

        std::cout << "Trovato spento" << std::endl; //debug
        Dispositivo dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);

        setDeviceTimer(dispositivo, startTime, endTime);                    
        dispositiviDaAccendere.insert(dispositivo);

    }else{
        //se non esiste lo creo con CreaDispositivo::creaDispositivo
        std::cout << "Non trovato, creo nuovo" << std::endl; //debug
        Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime);
        dispositiviDaAccendere.insert(*dispositivo);
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
                std::cout << "Non puoi tornare indietro nel tempo!" << std::endl;
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
                if(!dispositiviDaAccendere.isEmpty()){
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
                        std::cout << "Superato il limite di kilowatt, tolgo il primo dispositivo non sempre acceso" << std::endl;
                    }

                    std::cout<< "Consumo attuale: " << dispositiviAccesi.getConsumoAttuale(currentTime)<<std::endl; //debug
                    
                    std::cout << "\nACCESI " << dispositiviAccesi.showAll() << std::endl; //debug
                    std::cout << "\nDA ACCENDERE " << dispositiviDaAccendere.showAll() << std::endl; //debug
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

                int startTime = convertTimeToInt(arg2);
                if(checkWrongTimeFormat("startTime", startTime)) return 1;

                int endTime = -1;
                if(v.size() == 4){//controllo se l'utente ha inserito un tempo di spegnimento
                    endTime = convertTimeToInt(v.at(3));
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
        dispositiviAccesi.removeTimer(nomeDispositivo, currentTime);
    }
    else if (command == "show"){//[WIP]
        //mostro tutti i dispositivi (attivi e non) con produzione/consumo di ciascuno dalle 00:00 fino a quando ho inviato il comando show
        //inoltre mostro produzione/consumo totale del sistema dalle 00:00 a quando ho inviato il comando show
        if (v.size() < 2){
            std::cout << "ACCESI" << std::endl << dispositiviAccesi.showAll() << std::endl;
            std::cout << "DA ACCENDERE" << std::endl << dispositiviDaAccendere.showAll() << std::endl;
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
        }
    }
    try{
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
    }catch(const std::invalid_argument& e){
        return -1;
    }
}

std::string Interfaccia::convertIntToTime(int minuti)  {
    int ore = minuti / 60 % 24;
    int min = minuti % 60;
    return (ore < 10 ? "0" : "") + std::to_string(ore) + ":" + (min < 10 ? "0" : "") + std::to_string(min);
}
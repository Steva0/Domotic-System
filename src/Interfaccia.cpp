#include "../include/Interfaccia.h"
#include "../include/RicercaDispositivo.h"
#include "../include/Dispositivo.h"
#include <sstream>
#include <vector>

Interfaccia::Interfaccia(){}//[WIP]

Interfaccia::~Interfaccia(){}//[WIP]

void incompleteOrWrongCommand(std::string command, bool recursive=false) {
    if(!recursive) std::cout << "Comando incompleto o non valido!\n";//[WIP] si potrebbe stampare la sintassi del comando inserito
    if(command == "set time"){
        std::cout << "Sintassi corretta: set time $TIME \n";
    }
    if(command == "set device"){
        std::cout << "Sintassi corretta: set $DEVICE_NAME on/off\n";
        std::cout << "                   set $DEVICE_NAME ${START} [${STOP}]\n";
    }
    if(command == "rm"){
        std::cout << "Sintassi corretta: rm $DEVICE_NAME\n";
    }
    if(command == "reset"){
        std::cout << "Sintassi corretta: reset time/timers/all\n";
    }
    if(command == "set"){
        incompleteOrWrongCommand("set time", true);
        incompleteOrWrongCommand("set device", true);
    }
    if(command == "fullCommands"){
        std::cout << "Comandi disponibili:\n";
        std::cout << "  set time $TIME\n";
        std::cout << "  set $DEVICE_NAME on/off\n";
        std::cout << "  set $DEVICE_NAME ${START} [${STOP}]\n";
        std::cout << "  rm $DEVICE_NAME\n";
        std::cout << "  show\n";
        std::cout << "  reset time/timers/all\n";
    }
    return;
}

bool checkWrongTimeFormat(std::string timeType, int time) {
    if(time == -1){
        std::cout << "Formato orario [" << timeType << "] non valido!\n";
        return true;
    }
    return false;
}

void Interfaccia::parseAndRunCommand(std::string userInput) {
    
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
    if(!commandOk){
        incompleteOrWrongCommand("fullCommands");
        return;
    }
    if (command == "help")
    {
        incompleteOrWrongCommand("fullCommands", true);
    }
    
    if (command == "set") {     
        if(v.size() < 2){
            incompleteOrWrongCommand("set");
            return;
        }
        std::string arg = v.at(1);
        if (arg == "time") {
            
            if(v.size() < 3){
                incompleteOrWrongCommand("set time");
                return;
            }

            int wantedTime = convertTimeToInt(v.at(2));

            if (checkWrongTimeFormat("wantedTime", wantedTime)) return;

            if(currentTime > wantedTime){
                throw std::invalid_argument("Non puoi tornare indietro nel tempo!");
            }

            // Cambiare tempo usando set time, minuto per minuto usando un ciclo while, controllo tempo spegnimento, accensione, controllo i kilowatt, 
            // se ho superato i kilowatt tolgo il primo dispositivo che non sia sempre acceso (isSempreAcceso)

            
            while(currentTime <= wantedTime){
                //ciclo per minuto

                //controllo se ci sono dispositivi da spegnere
                try{
                    std::vector<Dispositivo*> dispositiviTempSpenti = dispositiviAccesi.removeAllDispositiviOff(currentTime);
                    for(Dispositivo* disp : dispositiviTempSpenti){
                        disp->incrementaTempoAccensione(currentTime - disp->getOrarioAccensione());
                        dispositiviSpenti.insert(*disp);
                    }
                }catch(const std::exception& e){}

                //controllo di non aver superato i kilowatt
                if(dispositiviAccesi.getConsumoAttuale(currentTime) > MAX_KILOWATT){
                    std::cout << "Superato il limite di kilowatt, tolgo il primo dispositivo non sempre acceso\n";
                    Dispositivo* disp = dispositiviAccesi.removeFirst();
                    dispositiviSpenti.insert(*disp);
                }

                std::cout << "Time: " << currentTime << std::endl; //debug
                std::cout << "accesi " << dispositiviAccesi.showAll() << std::endl; //debug
                std::cout << "spenti " << dispositiviSpenti.showAll() << std::endl; //debug
                currentTime++;
            }
            
        }else{
            if(v.size() < 3){
                incompleteOrWrongCommand("set device");
                return;
            }
            std::string nomeDispositivo = RicercaDispositivo::ricercaDispositivoSimile(arg, dispositiviPredefiniti);
            std::string arg2 = v.at(2);

            if (arg2 == "on"){
                //accendo il dispositivo
                if(dispositiviAccesi.contains(nomeDispositivo)){
                    std::cout << "Dispositivo gia' acceso!";
                }else{
                    if(dispositiviSpenti.contains(nomeDispositivo)){
                        std::cout << "Trovato spento\n"; //debug
                        Dispositivo* dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
                        dispositivo->setOrarioAccensione(currentTime);
                        dispositiviAccesi.insert(*dispositivo);
                    }else{
                        std::cout << "Non trovato, creo nuovo\n"; //debug
                        Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, currentTime);
                        dispositiviAccesi.insert(*dispositivo);
                        std::cout << dispositiviAccesi << std::endl; //debug
                    }
                }
                
            }         //se accendo, salvo anche il tempo di accensione, maxtime il tempo di spegnimento
            else if (arg2 == "off"){
                //spengo il dispositivo
                //devo salvare il tempo totale di accensione = currentTime-startTime, e sposto su array dispositivi spenti

                if(dispositiviAccesi.contains(nomeDispositivo)){
                    std::cout << "Trovato acceso\n"; //debug
                    Dispositivo* dispositivo = dispositiviAccesi.removeDispositivoName(nomeDispositivo);

                    dispositivo->setOrarioSpegnimento(currentTime);

                    dispositivo->incrementaTempoAccensione(currentTime - dispositivo->getOrarioAccensione());
                    dispositiviSpenti.insert(*dispositivo);
                }else{
                    throw std::invalid_argument("Dispositivo gia' spento!");    //ci penso dopo [WIP]
                }
            }   
            else{//voglio impostare un timer
            //cerco se esiste gia il dispositivo
            //se non esiste lo creo con CreaDispositivo::creaDispositivo
            //se esiste modifico il tempo di start e di end del dispositivo usando setOrarioAccensione(int minuti) setOrarioSpegnimento(int minuti)
            //e se uso funzione insert sul dispositivo per metterlo nella lista dei dispositivi accesi/dovranno accendersi 
            //e lo tolgo dall array dei dispositivi spenti
                int startTime = convertTimeToInt(arg2);
                if(checkWrongTimeFormat("startTime", startTime)) return;

                int endTime = -1;
                if(v.size() == 4){//controllo se l'utente ha inserito un tempo di spegnimento
                    endTime = convertTimeToInt(v.at(3));
                    if(checkWrongTimeFormat("endTime", endTime)) return;
                }

                if(dispositiviAccesi.contains(nomeDispositivo)){
                    //se il dispositivo ha un tempo di accensione ma è ancora spento (currentTime < tempo di accensione), allora sovrascrivo i tempi di accensione e spegnimento
                    //se il dispositivo ha un tempo di accensione ma è già acceso (currentTime > tempo di accensione)

                    Dispositivo* dispositivo = dispositiviAccesi.removeDispositivoName(nomeDispositivo);

                    if(dispositivo->isCP()){
                        endTime = startTime + dispositivo->getDurataCiclo();
                    }

                    if(dispositivo->getOrarioAccensione() > currentTime){

                        dispositivo->setOrarioSpegnimento(endTime);
                        dispositivo->setOrarioAccensione(startTime);
                        
                        dispositiviAccesi.insert(*dispositivo);

                    }else if(dispositivo->getOrarioAccensione() <= currentTime){

                        dispositivo->setOrarioSpegnimento(currentTime-1);
                        dispositivo->incrementaTempoAccensione(dispositivo->getOrarioSpegnimento() - dispositivo->getOrarioAccensione());
                        
                        dispositivo->setOrarioSpegnimento(endTime);                      
                        dispositivo->setOrarioAccensione(startTime);
                        
                        dispositiviAccesi.insert(*dispositivo);
                    }

                }else if(dispositiviSpenti.contains(nomeDispositivo)){
                    std::cout << "Trovato spento\n"; //debug
                    Dispositivo* dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
                    dispositivo->setOrarioAccensione(startTime);
                    dispositivo->setOrarioSpegnimento(endTime);
                    dispositiviAccesi.insert(*dispositivo);
                }else{
                    std::cout << "Non trovato, creo nuovo\n"; //debug
                    Dispositivo* dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime);
                    dispositiviAccesi.insert(*dispositivo);
                }
            }
        }
    }

    else if (command == "rm") {
        if(v.size() < 2){
            incompleteOrWrongCommand("rm");
            return;
        }
        //rimuovo timer da un dispositivo, mettendo a maxtime il tempo di spegnimento
        std::string nomeDispositivo = RicercaDispositivo::ricercaDispositivoSimile(v.at(1), dispositiviPredefiniti);
        dispositiviAccesi.removeTimer(nomeDispositivo);
    } 

    else if (command == "show"){
        //mostro tutti i dispositivi (attivi e non) con produzione/consumo di ciascuno dalle 00:00 fino a quando ho inviato il comando show
        //inoltre mostro produzione/consumo totale del sistema dalle 00:00 a quando ho inviato il comando show
        std::cout << dispositiviAccesi.showAll() << std::endl;
        std::cout << dispositiviSpenti.showAll() << std::endl;
    }

    else if (command == "reset"){
        if(v.size() < 2){
            incompleteOrWrongCommand("reset");
            return;
        }
        std::string arg = v.at(1);
        if (arg == "time") {
            //riporto tempo a 00:00, tutti i dispositivi alle condizioni iniziali (?), i timer vengono mantenuti
        } else if (arg == "timers") {
            //tolgo tutti i timer impostati e i dispositivi restano nello stato corrente (acceso/spento)
            dispositiviAccesi.removeAllTimers();
        } else if (arg == "all") {
            //riporto tutto alle condizioni iniziali (orario a 00:00, tolgo tutti i timer, tutti i dispositivi spenti)
            dispositiviAccesi.removeAllTimers();
            dispositiviAccesi.removeAllDispositiviOff(Dispositivo::MAX_MINUTI_GIORNATA);
        }
    }
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

    if(explicitTime){
        while (getline(ss, s, ':')) {
            v.push_back(s);
        }
        int hours = std::stoi(v.at(0));
        int minutes = std::stoi(v.at(1));
        if(hours < 0 || hours > 23 || minutes < 0 || minutes > 59){
            return -1;
        }
        std::cout<<hours * 60 + minutes<<std::endl;     //debug
        return hours * 60 + minutes;
    }else{
        int hours = std::stoi(time);
        if(hours < 0 || hours > 23){
            return -1;
        }
        std::cout<<hours*60<<std::endl;                 //debug
        return hours * 60;
    }
}
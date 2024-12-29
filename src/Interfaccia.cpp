#include "../include/Interfaccia.h"
#include "../include/RicercaDispositivo.h"
#include "../include/Dispositivo.h"
#include <sstream>
#include <vector>

void Interfaccia::parseAndRunCommand(std::string userInput) {
    std::string s;
    std::stringstream ss(userInput);
    std::vector<std::string> v;

    // divido l'input in argomenti separati da spazi
    while (getline(ss, s, ' ')) {
         v.push_back(s);
    }

    std::string command = v.at(0);
    if (command == "set") {
        std::string arg = v.at(1);
        if (arg == "time") {
            int wantedTime = convertTimeToInt(v.at(2));

            if(currentTime > wantedTime){
                throw std::invalid_argument("Non puoi tornare indietro nel tempo!");
            }

            // Cambiare tempo usando set time, minuto per minuto usando un ciclo while, controllo tempo spegnimento, accensione, controllo i kilowatt, 
            // se ho superato i kilowatt tolgo il primo dispositivo che non sia sempre acceso (isSempreAcceso)

            while(currentTime < wantedTime){
                //ciclo per minuto
                currentTime++;
            }
            
        }else{

            std::string nomeDispositivo = RicercaDispositivo::ricercaDispositivoSimile(arg, dispositiviPredefiniti);
            std::string arg2 = v.at(2);

            if (arg2 == "on"){
                //accendo il dispositivo
                if(dispositiviAccesi.contains(nomeDispositivo)){
                    throw std::invalid_argument("Dispositivo gia' acceso!");    //ci penso dopo [WIP]
                }else{
                    if(dispositiviSpenti.contains(nomeDispositivo)){
                        Dispositivo* dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
                        dispositivo->setOrarioAccensione(currentTime);
                        dispositiviAccesi.insert(*dispositivo);
                    }else{
                        Dispositivo dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, currentTime);
                        dispositiviAccesi.insert(dispositivo);
                    }
                }
                
            }         //se accendo, salvo anche il tempo di accensione, maxtime il tempo di spegnimento
            else if (arg2 == "off"){
                //spengo il dispositivo
                //devo salvare il tempo totale di accensione = currentTime-startTime, e sposto su array dispositivi spenti

                if(dispositiviAccesi.contains(nomeDispositivo)){
                    Dispositivo* dispositivo = dispositiviAccesi.removeDispositivoName(nomeDispositivo);
                    dispositivo->setOrarioSpegnimento(currentTime);
                    dispositivo->incrementaTempoAccensione(currentTime-dispositivo->getTempoAccensione());
                    dispositiviSpenti.insert(*dispositivo);
                }else{
                    throw std::invalid_argument("Dispositivo gia' spento!");    //ci penso dopo [WIP]
                }
            }   
            else{//voglio impostare un timer
            //cerco se esiste gia il dispositivo
            //se non esiste lo creo con CreaDispositivo::creaDispositivo
            //se esiste modifico il tempo di start e di end del dispositivo usando setOrarioAccensione(int minuti) setOrarioSpegnimento(int minuti)
            //e se uso funzione insert sul dispositivo per meterlo nella lista dei dispositivi accesi/dovranno accednersi 
            //e lo tolgo dall array dei dispositivi spenti
                int startTime = convertTimeToInt(arg2);
                int endTime = -1;

                if(v.size() == 4){//controllo se l'utente ha inserito un tempo di spegnimento
                    endTime = convertTimeToInt(v.at(3));
                }

                if(dispositiviAccesi.contains(nomeDispositivo)){
                    //DOBBIAMO CAPIRE COME FARE QUI
                }else if(dispositiviSpenti.contains(nomeDispositivo)){
                    Dispositivo* dispositivo = dispositiviSpenti.removeDispositivoName(nomeDispositivo);
                    dispositivo->setOrarioAccensione(startTime);
                    dispositivo->setOrarioSpegnimento(endTime);
                    dispositiviAccesi.insert(*dispositivo);
                }else{
                    Dispositivo dispositivo = CreaDispositivo::creaDispositivo(nomeDispositivo, startTime, endTime);
                    dispositiviAccesi.insert(dispositivo);
                }
            }
        }
    }

    else if (command == "rm") {
        //rimuovo timer da un dispositivo, mettendo a maxtime il tempo di spegnimento
        std::string nomeDispositivo = RicercaDispositivo::ricercaDispositivoSimile(v.at(1), dispositiviPredefiniti);
        dispositiviAccesi.removeTimer(nomeDispositivo);
    } 

    else if (command == "show"){
        //mostro tutti i dispositivi (attivi e non) con produzione/consumo di ciascuno dalle 00:00 fino a quando ho inviato il comando show
        //inoltre mostro produzione/consumo totale del sistema dalle 00:00 a quando ho inviato il comando show
    }

    else if (command == "reset"){
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

    while (getline(ss, s, ':')) {
         v.push_back(s);
    }

    int hours = std::stoi(v.at(0));
    int minutes = std::stoi(v.at(1));

    return hours * 3600 + minutes;
}
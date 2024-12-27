#include "../header/Interfaccia.h"
#include <sstream>
#include <vector>

void Interfaccia::parseCommand(std::string userInput) {
    std::string s;
    std::stringstream ss(userInput);
    std::vector<std::string> v;

    while (getline(ss, s, ' ')) {
         v.push_back(s);
    }

    std::string command = v.at(0);
    if (command == "set") {
        std::string arg = v.at(1);
        if (arg == "time") {
            std::string time = v.at(2);
            int timeInt = convertTimeToInt(time);
            // Cambiare tempo usando set time, minuto per minuto usando un ciclo while, controllo tepo spegnimento, accensione, controllo i kilowatt, 
            // se ho superato i kilowat tolgo il primo dispositivo che non sia sempre acceso (isSempreAcceso)
        }else{
            std::string nomeDispositivo = arg;
            std::string arg2 = v.at(2);

            if (arg2 == "on"){}  //se accendo, salvo anche il tempo di accensione, maxtime il tempo di spegnimento
            else if (arg2 == "off"){}//se spengo devo salvare il tempo totale di accensione = currentTime-startTime, e sposto su array dispositivi spenti

            else{//voglio impostare un timer
                int startTime = convertTimeToInt(arg2);
                if(v.size() == 4){
                    int endTime = convertTimeToInt(v.at(3));
                }

                //cerco se esiste gia il dispositivo
                //se non esiste lo creo con CreaDispositivo::creaDispositivo
                //se esiste modifico il tempo di start e di end del dispositivo usando setOrarioAccensione(int minuti) setOrarioSpegnimento(int minuti)
                //e se uso funzione insert sul dispositivo per meterlo nella lista dei dispositivi accesi/dovranno accednersi 
                //e lo tolgo dall array dei dispositivi spenti
            }


        }
    }

    else if (command == "rm") {} //metto a maxtime il tempo di spegnimento

    else if (command == "show"){}

    else if (command == "reset"){
        std::string arg = v.at(1);
        if (arg == "time") {
            // Handle reset time
        } else if (arg == "timers") {
            // Handle reset timers
        } else if (arg == "all") {
            // Handle reset all
        }
    }
}

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
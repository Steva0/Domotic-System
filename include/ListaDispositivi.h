//Michele Stevanin
#ifndef LISTA_DISPOSITIVI_H
#define LISTA_DISPOSITIVI_H

#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

// Struttura dei dispositivi predefiniti: {nome, (potenza, durataCiclo, sempreAcceso)}
const std::vector<std::pair<std::string, std::tuple<double, int, bool>>> dispositiviPredefiniti = {
    {"Impianto fotovoltaico", {1.5, 0, false}}, 
    {"Lavatrice", {-2.0, 110, false}},         
    {"Lavastoviglie", {-1.5, 195, false}},     
    {"Pompa di calore + termostato", {-2.0, 0, false}},  
    {"Tapparelle elettriche", {-0.3, 1, false}},  
    {"Scaldabagno", {-1.0, 0, false}},  
    {"Frigorifero", {-0.4, 0, true}},  
    {"Forno a microonde", {-0.8, 2, false}},  
    {"Asciugatrice", {-0.5, 60, false}},  
    {"Televisore", {-0.2, 60, false}},
    //add nostre
    {"Centrale nucleare", {1000000000.0, 0, false}},
    {"Computer", {-0.8, 0, false}},             
    {"Server", {-2.0, 0, false}},               
    {"Aspirapolvere", {-0.9, 0, false}},        
    {"Forno", {-1.6, 0, false}},                
    //{"Wii", {-0.4, 0, false}},                  
    {"Piano a induzione", {-1.5, 0, false}},    
    {"Allarme", {-0.4, 0, false}},              
    {"Citofono", {-0.2, 0, true}},              
    {"Condizionatore", {-1.8, 300, false}},     
    {"Stufa elettrica", {-2.5, 0, false}},      
    {"Lampada LED", {-0.05, 0, false}},         
    {"Proiettore", {-1.0, 120, false}},         
    {"Stereofono", {-0.3, 30, false}},          
    {"Macchina del caffè", {-0.2, 2, false}},   
    {"Friggitrice ad aria", {-1.2, 100, false}}, 
    {"Deumidificatore", {-1.0, 50, true}},      
    {"Ventilatore", {-0.6, 0, false}},          
    {"Tostapane", {-0.3, 5, false}},            
    {"Piastra per capelli", {-0.1, 10, false}}, 
    {"Microfono", {-0.05, 0, false}},           
    {"Amplificatore", {-0.8, 50, false}},       
    {"Tagliaerba elettrico", {-1.5, 0, false}}, 
    {"Macchina per il pane", {-0.8, 150, false}}, 
    {"Scanner", {-0.4, 5, false}},             
    {"Stampante", {-0.6, 4, false}},           
    {"Frigorifero smart", {-0.5, 0, true}},     
    {"Climatizzatore portatile", {-1.2, 0, false}},
    {"Lampada da tavolo", {-0.05, 0, false}},       
    {"Congelatore", {-0.6, 0, false}},          
    {"Macchina per il gelato", {-0.4, 0, false}}, 
    {"Frullatore", {-0.2, 0, false}}      
};


// Mappa per tenere traccia del numero di serie specifico per ogni tipo di dispositivo
extern std::unordered_map<std::string, int> numeroSerieDispositivi;

#endif // !LISTA_DISPOSITIVI_H


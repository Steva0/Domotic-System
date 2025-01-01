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
    {"Impianto nucleare", {1000000.0, 0, false}},         //add nostre
    {"Lavatrice", {-2.0, 110, false}},         
    {"Lavastoviglie", {-1.5, 195, false}},     
    {"Pompa di calore + termostato", {-2.0, 0, false}},  
    {"Tapparelle elettriche", {-0.3, 1, false}},  
    {"Scaldabagno", {-1.0, 0, false}},  
    {"Frigorifero", {-0.4, 0, true}},  
    {"Forno a microonde", {-0.8, 2, false}},  
    {"Asciugatrice", {-0.5, 60, false}},  
    {"Televisore", {-0.2, 60, false}},
    {"Computer", {-0.8, 0, false}},               //add nostre
    {"Server", {-2.0, 0, false}},                 //add nostre
    {"Aspirapolvere", {-0.9, 0, false}},          //add nostre
    {"Forno", {-1.6, 0, false}},                  //add nostre
    {"Wii", {-0.4, 0, false}},                    //add nostre
    {"Piano a induzione", {-1.5, 0, false}},      //add nostre
    {"Allarme", {-0.4, 0, false}},                //add nostre
    {"Citofono", {-0.2, 0, true}}
};

// Mappa per tenere traccia del numero di serie specifico per ogni tipo di dispositivo
extern std::unordered_map<std::string, int> numeroSerieDispositivi;

#endif // !LISTA_DISPOSITIVI_H


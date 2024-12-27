#ifndef LISTA_DISPOSITIVI_H
#define LISTA_DISPOSITIVI_H

#include <string>
#include <tuple>
#include <vector>

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
    {"Televisore", {-0.2, 0, false}} 
};

#endif // !LISTA_DISPOSITIVI_H

#include "../include/LinkedList.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    try {
        // Creazione della lista collegata
        LinkedList lista;
    
        // Creazione dispositivi di test
        Dispositivo d1("Lampada", 1, 100, 5, 10);
        Dispositivo d2("PC", 2, 200, 8, 12);
        Dispositivo d3("TV", 3, 150, 6, 9);
    
        // Inserimento dispositivi
        lista.insert(d1);
        lista.insert(d2);
        lista.insert(d3);
    
        // Visualizzazione iniziale della lista
        std::cout << "Lista iniziale:\n" << lista << std::endl;
    
        // Rimozione per nome
        lista.removeDispositivoName("Lampada");
        std::cout << "Lista dopo rimozione del dispositivo 'Lampada':\n" << lista.showAll() << std::endl;
    
        // Rimozione per ID
        lista.removeDispositivoId(2);
        std::cout << "Lista dopo rimozione del dispositivo con ID 2:\n" << lista.showAll() << std::endl;
    
        std::cout << "Lista dopo rimozione" << std::endl;
        lista.removeDispositivoName("TV");
    
        // Visualizzazione finale della lista
        std::cout << "Lista finale:\n" << lista.showAll() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << std::endl;
    }
    return 0;
}

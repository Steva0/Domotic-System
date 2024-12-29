#include "../include/LinkedList.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    try {
        LinkedList lista;
        std::cout << "Test: Operazioni su lista vuota" << std::endl;

        try {
            lista.removeDispositivoName("Test");
        } catch (const std::out_of_range& e) {
            std::cerr << "Errore: " << e.what() << std::endl;
            return 1;
        }

        try {
            lista.removeDispositivoId(1);
        } catch (const std::out_of_range& e) {
            std::cerr << "Errore: " << e.what() << std::endl;
            return 2;
        }

        try {
            lista.show("Test");
        } catch (const std::out_of_range& e) {
            std::cerr << "Errore: " << e.what() << std::endl;
            return 3;
        }

        // Creazione dispositivi di test
        Dispositivo d1("Lampada", 1, 100, 5, 10);
        Dispositivo d2("PC", 2, 200, 8, 12);
        Dispositivo d3("TV", 3, 150, 6, 9);

        std::cout << "Test: Inserimenti" << std::endl;
        try {
            lista.insert(d1);
            lista.insert(d2);
            lista.insert(d3);
        } catch (const std::exception& e) {
            std::cerr << "Errore durante l'inserimento: " << e.what() << std::endl;
            return 4;
        }

        std::cout << "Lista attuale:\n" << lista.showAll() << std::endl;

        std::cout << "Test: Rimozione per nome" << std::endl;
        try {
            lista.removeDispositivoName("Lampada");
        } catch (const std::invalid_argument& e) {
            std::cerr << "Errore: " << e.what() << std::endl;
            return 5;
        }

        std::cout << "Lista dopo rimozione:\n" << lista.showAll() << std::endl;

        std::cout << "Test: Rimozione per ID" << std::endl;
        try {
            lista.removeDispositivoId(2);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Errore: " << e.what() << std::endl;
            return 6;
        }

        std::cout << "Lista dopo rimozione:\n" << lista.showAll() << std::endl;

        std::cout << "Test: Rimozione dispositivi spenti" << std::endl;
        try {
            auto spenti = lista.removeAllDispositiviOff(10);
            std::cout << "Dispositivi rimossi:\n";
            for (const auto& disp : spenti) {
                std::cout << disp->getNome() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Errore: " << e.what() << std::endl;
            return 7;
        }

        std::cout << "Lista finale:\n" << lista.showAll() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Errore critico: " << e.what() << std::endl;
        return 99;
    }

    return 0;
}

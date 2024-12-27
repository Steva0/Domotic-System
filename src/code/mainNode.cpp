//Alberto Bortoletto
#include "Node.h"
#include <iostream>

using namespace std;

int main()
{
    Dispositivo Frigorifero("Frigorifero", -0.4, 0);
    Node nodo1(Frigorifero);

    cout << nodo1.getDispositivo().getNome();
    cout << nodo1.getNext().getDispositivo().getNome();
    cout << nodo1.getPrev().getDispositivo().getNome();
    
}
//Alberto Bortoletto
#include "../header/Node.h"
#include <iostream>

using namespace std;

int main()
{
    Dispositivo Frigorifero = CreaDispositivo::creaDispositivo("Frigrifero", -0.4, 0);
    Node nodo1(Frigorifero);

    cout << nodo1.getDispositivo().getNome() << endl;
    cout << nodo1.getNext().getDispositivo().getNome() << endl;
    cout << nodo1.getPrev().getDispositivo().getNome() << endl;
    
}
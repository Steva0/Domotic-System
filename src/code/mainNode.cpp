//Alberto Bortoletto
#include "../header/LinkedList.h"
#include <iostream>

using namespace std;

int main()
{
    Dispositivo Frigorifero = CreaDispositivo::creaDispositivo("Frigrifero", 2);
    Dispositivo Lavatrice = CreaDispositivo::creaDispositivo("Lavtrice", 9);
    Dispositivo ScaldaBagno = CreaDispositivo::creaDispositivo("Scalda bagno", 1);
    LinkedList list = LinkedList();

    list.insert(Frigorifero);
    cout << list << endl;

    list.insert(Lavatrice);
    cout << list << endl;

    list.insert(ScaldaBagno);
    cout << list << endl;
    
    
}
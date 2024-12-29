//Alberto Bortoletto
#include "../include/LinkedList.h"
#include <iostream>

using namespace std;

int main()
{
    try {
        Dispositivo Frigorifero = CreaDispositivo::creaDispositivo("Frigrifero", 76);        //Manuale 
        Dispositivo Lavatrice = CreaDispositivo::creaDispositivo("Lavtrice", 8);            //110 min 
        Dispositivo ScaldaBagno = CreaDispositivo::creaDispositivo("Scalda bagno", 9);       //Manuale
        Dispositivo Televisore = CreaDispositivo::creaDispositivo("Televisre", 4);          //60 min 
        Dispositivo Asciugatrice = CreaDispositivo::creaDispositivo("Asciugatrice", 2);     //60 min
        Dispositivo FornoMicroonde = CreaDispositivo::creaDispositivo("Forno a microonde", 1);   //2 min
        
        LinkedList list = LinkedList();

        list.insert(Frigorifero);
        cout << list << endl;

        list.insert(Lavatrice);
        cout << list << endl;

        list.insert(ScaldaBagno);
        cout << list << endl;

        list.insert(Televisore);
        cout << list << endl;

        list.insert(FornoMicroonde);
        cout << list << endl;

        list.insert(Asciugatrice);
        cout << list << endl;

        list.removeDispositivoName("Scaldabagno");
        cout << list << endl;

        cout << Televisore.getOrarioSpegnimento() << endl;

        list.removeAllDispositiviOff(5);
        cout << list << endl;

        list.removeTimer("Asciugatrice");
        cout << list << endl;
        cout << Asciugatrice.getOrarioSpegnimento() << endl;
    }
    catch (exception& e) {
        return 1;
    }
    return 0;
}
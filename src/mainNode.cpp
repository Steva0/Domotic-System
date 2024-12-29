//Alberto Bortoletto
#include "../include/LinkedList.h"
#include <iostream>

using namespace std;

int main()
{
    Dispositivo Frigorifero = CreaDispositivo::creaDispositivo("Frigrifero", 76);        //Manuale 
    Dispositivo Lavatrice = CreaDispositivo::creaDispositivo("Lavtrice", 8);            //110 min 
    Dispositivo ScaldaBagno = CreaDispositivo::creaDispositivo("Scalda bagno", 9);       //Manuale
    Dispositivo Televisore = CreaDispositivo::creaDispositivo("Televisre", 4);          //60 min 
    Dispositivo Asciugatrice = CreaDispositivo::creaDispositivo("Asciugatrice", 2);     //60 min
    Dispositivo FornoMicroonde = CreaDispositivo::creaDispositivo("Forno a microonde", 1);   //2 min
    Dispositivo Televisore2 = CreaDispositivo::creaDispositivo("Televisre", 98);          //60 min
    
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

    list.insert(Televisore2);
    cout << list << endl;

    list.removeDispositivoName("Scaldabagno");
    cout << list << endl;

    int id = 2;

    list.removeDispositivoId(id);
    cout << "Ho rimosso l'id " << id << endl << list << endl;

    cout << Televisore.getOrarioSpegnimento() << endl;

    list.removeAllDispositiviOff(1);
    cout << list << endl;

    list.removeTimer("Asciugatrice");
    cout << list << endl;
    cout << Asciugatrice.getOrarioSpegnimento() << endl;

    cout << list.showAll();

    return 0;
}
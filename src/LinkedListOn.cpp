//Alberto Bortoletto
#include "../include/LinkedListOn.h"

LinkedListOn::LinkedListOn(): LinkedList(), status{"ACCESO"}
{ }

LinkedListOn::LinkedListOn(Dispositivo& dispositivo): LinkedList(dispositivo), status{"ACCESO"}
{ }

void LinkedListOn::incrementTimeOn()
{
    LinkedList::checkEmpty();

    std::shared_ptr<Node> current = head;
    while(current)
    {   
        current->disp->incrementaTempoAccensione(1);
        current = current->next;
    }
}

double LinkedListOn::getConsumoAttuale(int currentTime) const
{
    if(isEmpty())
    {
        return 0;
    }

    if(currentTime < 0 || currentTime > Dispositivo::MAX_MINUTI_GIORNATA)
    {
        throw std::invalid_argument("Orario non valido!");
    }

    double consumoTotale = 0;
    std::shared_ptr<LinkedList::Node> current = head;
    while(current && current->disp->getOrarioAccensione() <= currentTime && currentTime < current->disp->getOrarioSpegnimento())
    {
        consumoTotale += current->disp->getPotenza();

        current = current->next;
    }
    return consumoTotale;
}

double LinkedListOn::getProdotta() const 
{
    if(isEmpty())
    {
        return 0;
    }

    double energiaProdotta = 0;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        if(current->disp->getPotenza() > 0)
        {
            energiaProdotta += current->disp->getPotenza();
        }
        current = current->next;
    }

    energiaProdotta = energiaProdotta < 0 ? -1*energiaProdotta : energiaProdotta;

    return energiaProdotta;
}

double LinkedListOn::getUsata() const 
{
    if(isEmpty())
    {
        return 0;
    }

    double energiaUtilizzata = 0;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        if(current->disp->getPotenza() < 0)
        {
            energiaUtilizzata += current->disp->getPotenza();
        }
        current = current->next;
    }

    energiaUtilizzata = energiaUtilizzata < 0 ? -1*energiaUtilizzata : energiaUtilizzata;

    return energiaUtilizzata;
}
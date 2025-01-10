//Alberto Bortoletto
#include "../include/LinkedListOn.h"

LinkedListOn::LinkedListOn(): LinkedList()
{ 
    status = "[ACCESO]";
}

LinkedListOn::LinkedListOn(Dispositivo& dispositivo): LinkedList(dispositivo)
{ 
    status = "[ACCESO]";
}

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

double LinkedListOn::getConsumoAttuale() const
{
    if(isEmpty())
    {
        return 0;
    }

    double consumoTotale = 0;
    std::shared_ptr<LinkedList::Node> current = head;
    while(current)
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
        if(current->disp->isGenerator())
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
        if(!current->disp->isGenerator())
        {
            energiaUtilizzata += current->disp->getPotenza();
        }
        current = current->next;
    }

    energiaUtilizzata = energiaUtilizzata < 0 ? -1*energiaUtilizzata : energiaUtilizzata;

    return energiaUtilizzata;
}
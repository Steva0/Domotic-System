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

std::vector<Dispositivo> LinkedListOn::removeAllDevicesOff(const int currentTime)
{
    checkEmpty();

    std::vector<Dispositivo> dispositiviSpenti;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        std::shared_ptr<Node> prossimo = current->next;
        if(!current->disp->isAcceso(currentTime))
        {
            dispositiviSpenti.push_back(*(current->disp.get()));
            std::shared_ptr<Node> temp = current;
            removeDispositivo(temp->disp->getNome());
        }
        current = prossimo;
    }

    return dispositiviSpenti;
}

void LinkedListOn::incrementTimeOn()
{
    if(isEmpty()) return;

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
    std::shared_ptr<Node> current = head;
    while(current)
    {
        consumoTotale += current->disp->getPotenza();

        current = current->next;
    }
    return consumoTotale;
}

double LinkedListOn::producedEnergy() const 
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

    return energiaProdotta/60;
}

double LinkedListOn::consumedEnergy() const 
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

    return energiaUtilizzata/60;
}
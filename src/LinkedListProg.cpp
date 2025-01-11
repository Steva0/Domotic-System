//Alberto Bortoletto
#include "../include/LinkedListProg.h"

LinkedListProg::LinkedListProg(): LinkedList()
{ 
    status = "[SPENTO - Si accendera' tra ";
}

LinkedListProg::LinkedListProg(Dispositivo& dispositivo): LinkedList(dispositivo)
{ 
    status = "[SPENTO - Si accendera' tra ";
}

std::vector<Dispositivo> LinkedListProg::removeDevicesToPowerOn(const int currentTime)
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    std::vector<Dispositivo> dispositiviAccesi;
    while(current)
    {
        std::shared_ptr<Node> prossimo = current->next;
        if(current->disp->getOrarioAccensione() == currentTime)
        {
            dispositiviAccesi.push_back(*(current->disp.get()));
            std::shared_ptr<Node> temp = current;
            removeDispositivo(temp->disp->getNome());
        }
        current = prossimo;        
    }

    return dispositiviAccesi;
}

std::string LinkedListProg::showAll(int currentTime) const
{
    if(isEmpty())
    {
        return "";
    }
    std::ostringstream statsStream;
    std::shared_ptr<Node> current = head;
    while (current)
    {
        if(current->disp->isGenerator())
        {
            statsStream << "Il dispositivo " << current->disp->getNome() << " ha prodotto ";
        }
        else
        {
            statsStream << "Il dispositivo " << current->disp->getNome() << " ha consumato ";
        }

        statsStream << std::fixed << std::setprecision(3) << std::fabs(current->disp->calcolaConsumoEnergetico()) << " kWh " << status << (current->disp->getOrarioAccensione() - currentTime) << " minuti]";

        current = current->next;

        if(current)
        {
            statsStream << "\n\t";
        }
    }
    return statsStream.str();
}
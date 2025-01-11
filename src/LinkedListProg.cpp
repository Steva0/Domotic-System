//Alberto Bortoletto
#include "../include/LinkedListProg.h"

//Costruttore di default che chiama LinkedList() e modifica la stringa status
LinkedListProg::LinkedListProg(): LinkedList()
{ 
    status = "[SPENTO - Si accendera' tra ";
}

//Costruttore con inserimento di un dispositivo in coda che chiama LinkedList(dispositivo) e modifica la stringa status
LinkedListProg::LinkedListProg(Dispositivo& dispositivo): LinkedList(dispositivo)
{ 
    status = "[SPENTO - Si accendera' tra ";
}

//Rimuove e ritorna tutti i dispositivi che vengono accesi all'orario corrente - Puo' lanciare un'eccezione std::out_of_range("Lista vuota!")
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

//Ritorna una stringa che mostra il consumo di ogni nodo
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
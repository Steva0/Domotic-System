//Alberto Bortoletto
#include "../include/LinkedListProg.h"

LinkedListProg::LinkedListProg(): LinkedList()
{ 
    status = "[SPENTO]";
}

LinkedListProg::LinkedListProg(Dispositivo& dispositivo): LinkedList(dispositivo)
{ 
    status = "[SPENTO]";
}

std::vector<Dispositivo> LinkedListProg::turnOnDevices(const int currentTime)
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    std::vector<Dispositivo> dispositiviAccesi;
    while(current)
    {
        std::shared_ptr<LinkedList::Node> prossimo = current->next;
        if(current->disp->getOrarioAccensione() == currentTime)
        {
            dispositiviAccesi.push_back(*(current->disp.get()));
            std::shared_ptr<Node> temp = current;
            removeDispositivoName(temp->disp->getNome());
        }
        current = prossimo;        
    }

    return dispositiviAccesi;
}
//Alberto Bortoletto
#include "../include/LinkedList.h"

LinkedList::Node::Node(const Dispositivo& data): disp{std::make_unique<Dispositivo> (data)}, next{nullptr}, prev{nullptr}
{ }

LinkedList::LinkedList(): head{nullptr}, tail{nullptr}
{ }

LinkedList::LinkedList(Dispositivo& dispositivo): head{nullptr}, tail{nullptr}
{
    insert(dispositivo);
}

LinkedList::LinkedList(const LinkedList& list): head{nullptr}, tail{nullptr}
{
    std::shared_ptr<Node> current = list.head;
    while(current)
    {
        insert(*(current->disp.get()));
        current = current->next;
    }
}

void LinkedList::insert(Dispositivo& dispositivo)
{
    std::shared_ptr<Node> newNode = std::make_shared<Node>(dispositivo);
    
    if(isEmpty())
    {
        head = tail = newNode;
        return;
    }

    //Trova il punto di inserimento
    std::shared_ptr<Node> current = head;
    while(current != nullptr && current->disp->getOrarioAccensione() <= dispositivo.getOrarioAccensione())
    {
        current = current->next;
    }

    if(current.get() == head.get())             //significa che lo aggiungo all'inizio di tutti, quindi prima di head
    {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    else if(current == nullptr)     //significa che lo aggiungo alla fine di tutti, quindi dopo tail
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    else
    {
        current->prev->next = newNode;
        newNode->prev = current->prev;

        newNode->next = current;
        current->prev = newNode;
    }
}

Dispositivo LinkedList::removeDispositivoName(const std::string& nome)
{
    checkEmpty();

    return removeNode(searchDispositivoName(nome));
}

Dispositivo LinkedList::removeDispositivoId(const int id)
{
    checkEmpty();
    
    return removeNode(searchDispositivoId(id));
}

std::vector<Dispositivo> LinkedList::removeAllDispositiviOff(const int currentTime)
{
    checkEmpty();

    std::vector<Dispositivo> dispositiviSpenti;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        std::shared_ptr<LinkedList::Node> prossimo = current->next;
        if(!current->disp->isAcceso(currentTime))
        {
            dispositiviSpenti.push_back(*(current->disp.get()));
            std::shared_ptr<Node> temp = current;
            removeDispositivoName(temp->disp->getNome());
        }
        current = prossimo;
    }

    return dispositiviSpenti;
}

std::vector<Dispositivo> LinkedList::removeAllForce(){
    try
    {
        return removeAllDispositiviOff(Dispositivo::MAX_MINUTI_GIORNATA);
    }
    catch (const std::out_of_range& e)
    {
        return std::vector<Dispositivo>();
    }
}

Dispositivo LinkedList::forceRemoveFirst()
{
    checkEmpty();

    std::shared_ptr<Node> current = head;

    if(head == tail)
    {
        head = tail = nullptr;
    }
    else
    {
        head = head->next;
        if (head) head->prev = nullptr;
    }
    
    return *current->disp.get();
}

Dispositivo LinkedList::removeFirst()
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    while(current && (current->disp->isSempreAcceso() || current->disp->isGenerator()))
    {
        current = current->next;
    }

    if(current == nullptr)
    {
        throw std::invalid_argument("Nessun dispositivo che non sia sempre acceso!");
    }

    return removeDispositivoName(current->disp->getNome());
}

std::vector<Dispositivo> LinkedList::turnOnDevices(const int currentTime)
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

void LinkedList::incrementTimeOn()
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    while(current)
    {   
        current->disp->incrementaTempoAccensione(1);
        current = current->next;
    }
}


double LinkedList::getConsumoAttuale(int currentTime) const{
    if(isEmpty())
    {
        return 0;
    }

    if(currentTime < 0 || currentTime > Dispositivo::MAX_MINUTI_GIORNATA)
    {
        throw std::invalid_argument("Orario non valido!");
    }

    double consumoTotale = 0;
    std::shared_ptr<Node> current = head;
    while(current && current->disp->getOrarioAccensione() <= currentTime && currentTime < current->disp->getOrarioSpegnimento())
    {
        consumoTotale += current->disp->getPotenza();

        current = current->next;
    }
    return consumoTotale;
}
std::vector<double> LinkedList::getConsumoAttualeDettagliato(int currentTime) const
{
    if(isEmpty())
    {
        return std::vector<double>(2, 0);
    }

    if(currentTime < 0 || currentTime > Dispositivo::MAX_MINUTI_GIORNATA)
    {
        throw std::invalid_argument("Orario non valido!");
    }

    std::vector<double> summaryEnergy(2, 0);
    std::shared_ptr<Node> current = head;
    while(current && current->disp->getOrarioAccensione() <= currentTime && currentTime < current->disp->getOrarioSpegnimento())
    {
        if(current->disp->getPotenza() > 0)
        {
            summaryEnergy.at(0) += current->disp->getPotenza();
        }
        else
        {
            summaryEnergy.at(1) += current->disp->getPotenza();
        }
        current = current->next;
    }

    return summaryEnergy;
}

void LinkedList::removeTimer(const std::string nome, const int currentTime)
{
    checkEmpty();

    std::shared_ptr<Node> current = searchDispositivoName(nome);

    if(current->disp->isAcceso(currentTime))
    {
        current->disp->setTimerOff();
    }
    else
    {
        current->disp->setOrarioAccensione(0);
        current->disp->setOrarioSpegnimento(0);
    }
        
}

void LinkedList::resetAllTimers(int currentTime)
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    while(current)
    {
        removeTimer(current->disp->getNome(), currentTime);
        current = current->next;
    }
}

void LinkedList::resetAll()
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    while(current)
    {
        current->disp->setOrarioAccensione(0);
        current->disp->setOrarioSpegnimento(0);
        current = current->next;
    }
}

double LinkedList::show(std::string nome) const
{
    checkEmpty();

    std::shared_ptr<Node> current = std::shared_ptr<Node>(searchDispositivoName(nome));
    return current->disp->calcolaConsumoEnergetico();
}

std::string LinkedList::inlinePrint() const
{
    std::string stats = "[";
    std::shared_ptr<Node> current = head;
    while(current)
    {
        stats += current->disp->getNome() + ", ";
        current = current->next;
    }
    return stats + "]";
}

std::string LinkedList::showAll() const
{
    if (isEmpty())
    {
        return "";
    }
    std::string stats;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        if(current->disp->isGenerator())
        {
            stats += "Il dispositivo " + current->disp->getNome() + " ha prodotto ";
        }else
        {
            stats += "Il dispositivo " + current->disp->getNome() + " ha consumato ";
        }

        stats += std::to_string(current->disp->calcolaConsumoEnergetico()) + "kW\n\t";

        current = current->next;
    }
    return stats;
}

std::string LinkedList::showAllDebug() const
{
    if (isEmpty())
    {
        return "[]";
    }
    std::string stats = "[\n";
    std::shared_ptr<Node> current = head;
    while(current)
    {
        stats += current->disp->showAllInfo() + "\n";
        current = current->next;
    }
    return stats + "]";
}

bool LinkedList::contains(const std::string nome) const
{
    try
    {
        searchDispositivoName(nome);
        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

bool LinkedList::isEmpty() const
{
    return (head.get() == nullptr);
}

std::ostream& operator<<(std::ostream& os, const LinkedList& list)
{
    if(list.isEmpty())
    {
        os << "Linked List is Empty!";
    }
    else 
    {
        os << list.inlinePrint();
    }

    return os;
}

void LinkedList::checkEmpty() const
{
    bool empty = isEmpty();
    if(empty)
    {
        throw std::out_of_range("Lista vuota!");
    }
}

std::shared_ptr<LinkedList::Node> LinkedList::searchDispositivoName(const std::string nome) const
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    while(current && current->disp->getNome() != nome)
    {
        current = current->next;
    }

    if(current == nullptr)
    {
        throw std::invalid_argument("Dispositivo non trovato!");
    }

    return current;
}

std::shared_ptr<LinkedList::Node> LinkedList::searchDispositivoId(const int id) const
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    while(current && current->disp->getId() != id)
    {
        current = current->next;
    }

    if(current == nullptr)
    {
        throw std::invalid_argument("Dispositivo non trovato!");
    }

    return current;
}

Dispositivo LinkedList::removeNode(std::shared_ptr<Node> current)
{
    if(current.get() == tail.get() && current.get() == head.get())
    {
        head = tail = nullptr;
    }
    else if (current.get() == head.get()) 
    {   

        if(head->next) head = head->next;
        if(head->prev) head->prev->next = nullptr;
        head->prev = nullptr;
    } 
    else if (current.get() == tail.get()) 
    {
        if(tail->prev) tail = tail->prev;
        if(tail->prev) tail->next->prev = nullptr;
        tail->next = nullptr;
    }
    else
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    return *current->disp.get();
}

LinkedList& LinkedList::operator=(const LinkedList& list)
{
    if (this == &list) {
        return *this;
    }

    head.reset();
    tail.reset();

    if (list.head) {
        std::shared_ptr<Node> current = list.head;
        while (current) {
            insert(*(current->disp));
            current = current->next;
        }
    }

    return *this;
}
#include "../include/LinkedList.h"

LinkedList::Node::Node(const Dispositivo& data): disp{std::make_unique<Dispositivo> (data)}, next{nullptr}, prev{nullptr}
{ }

LinkedList::LinkedList(): head{nullptr}, tail{nullptr}
{ }

LinkedList::LinkedList(Dispositivo& dispositivo): head{nullptr}, tail{nullptr}
{
    insert(dispositivo);
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

    std::shared_ptr<Node> current = std::make_shared<Node> (searchDispositivoName(nome));
    
    if (current.get() == head.get()) 
    {   
        head = head->next;
        head->prev->next = nullptr;
        head->prev = nullptr;
    } 
    else if (current.get() == tail.get()) 
    {
        tail = tail->prev;
        tail->next->prev = nullptr;
        tail->next = nullptr;
    }
    else 
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }
    
    return *current->disp.get();
}

Dispositivo LinkedList::removeDispositivoId(const int id)
{
    checkEmpty();

    std::shared_ptr<Node> current = std::make_shared<Node> (searchDispositivoId(id));
    
    if (current.get() == head.get()) 
    {   
        head = head->next;
        head->prev->next = nullptr;
        head->prev = nullptr;
    } 
    else if (current.get() == tail.get()) 
    {
        tail = tail->prev;
        tail->next->prev = nullptr;
        tail->next = nullptr;
    }
    else 
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }
    
    return *current->disp.get();
}

std::vector<Dispositivo> LinkedList::removeAllDispositiviOff(const int currentTime)
{
    checkEmpty();

    std::vector<Dispositivo> dispositiviSpenti;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        if(current->disp->getOrarioSpegnimento() <= currentTime)
        {
            dispositiviSpenti.push_back(*(current->disp.get()));
            std::shared_ptr<Node> temp = current;
            removeDispositivoName(temp->disp->getNome());
        }
        current = current->next;
    }

    return dispositiviSpenti;
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
    while(current && current->disp->isSempreAcceso())
    {
        current = current->next;
    }

    if(current == nullptr)
    {
        throw std::invalid_argument("Nessun dispositivo che non sia sempre acceso!");
    }

    return removeDispositivoName(current->disp->getNome());
}

double LinkedList::getConsumoAttuale(int currentTime) const
{
    bool empty = isEmpty();
    if(empty)
    {
        return 0;
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

void LinkedList::removeTimer(const std::string nome)
{
    checkEmpty();

    std::shared_ptr<Node> current = searchDispositivoName(nome);

    current->disp->setTimerOff();
}

void LinkedList::removeAllTimers()
{
    checkEmpty();

    std::shared_ptr<Node> current = head;
    while(current)
    {
        current->disp->setTimerOff();
        current = current->next;
    }
}

double LinkedList::show(std::string nome) const
{
    checkEmpty();

    std::shared_ptr<Node> current = std::shared_ptr<Node>(searchDispositivoName(nome));
    return current->disp->calcolaConsumoEnergetico();
}

std::string LinkedList::showAll() const
{
    std::string stats = "[";
    std::shared_ptr<Node> current = head;
    while(current)
    {
        stats += current->disp->showAllInfo() + ", \n";
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
        os << list.showAll();
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
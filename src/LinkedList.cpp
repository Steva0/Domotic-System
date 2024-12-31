#include "../include/LinkedList.h"

LinkedList::Node::Node(const Dispositivo& data): disp{new Dispositivo(data)}, prev{nullptr}, next{nullptr}
{ }

LinkedList::LinkedList(): head{nullptr}, tail{nullptr}
{ }

LinkedList::Node::~Node()
{
    prev = nullptr;
    next = nullptr;
    delete disp;
}

LinkedList::LinkedList(Dispositivo& dispositivo)
{ 
    head = tail = new Node(dispositivo);
}

void LinkedList::insert(Dispositivo& dispositivo)
{
    Node* newNode = new Node(dispositivo);
    if(isEmpty())
    {
        head = tail = newNode;
        return;
    }

    //Trova il punto di inserimento
    Node* current = head;
    while(current && current->disp->getOrarioAccensione() <= dispositivo.getOrarioAccensione())
    {
        current = current->next;
    }

    if(current == head)             //significa che lo aggiungo all'inizio di tutti, quindi prima di head
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
        connectBefore(current, newNode);
    }
}

Dispositivo LinkedList::removeDispositivoName(const std::string& nome)
{
    checkEmpty();

    Node* current = searchDispositivoName(nome);
    
    if (current == head) 
    {
        head = head->next;
        if (head) head->prev = nullptr;
    } 
    else if (current == tail) 
    {
        tail = tail->prev;
        if (tail) tail->next = nullptr;
    } 
    else 
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }
    
    return *current->disp;
}

Dispositivo LinkedList::removeDispositivoId(const int id)
{
    checkEmpty();

    Node* current = searchDispositivoId(id);

    if(current == head)
    {
        head = head->next;
        if (head) head->prev = nullptr;
    }
    else if(current == tail)
    {
        tail = tail->prev;
        if (tail) tail->next = nullptr;
    }
    else
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    return *current->disp;
}

std::vector<Dispositivo> LinkedList::removeAllDispositiviOff(const int currentTime)
{
    checkEmpty();

    std::vector<Dispositivo> dispositiviSpenti;
    Node* current = head;
    while(current)
    {
        if(current->disp->getOrarioSpegnimento() <= currentTime)
        {
            dispositiviSpenti.push_back(*(current->disp));
            Node* temp = current;
            removeDispositivoName(temp->disp->getNome());
        }
        current = current->next;
    }

    return dispositiviSpenti;
}

Dispositivo LinkedList::forceRemoveFirst()
{
    checkEmpty();

    Node* current = head;

    if(head == tail)
    {
        head = tail = nullptr;
    }
    else
    {
        head = head->next;
        if (head) head->prev = nullptr;
    }
    
    return *current->disp;
}

Dispositivo LinkedList::removeFirst()
{
    checkEmpty();

    Node* current = head;
    while(current && current->disp->isSempreAcceso())
    {
        current = current->next;
    }

    if(current == nullptr)
    {
        throw std::invalid_argument("Nessun dispositivo che non sia sempre acceso!");
    }

    if(current == head)
    {
        head = head->next;
        if (head) head->prev = nullptr;
    }
    else if(current == tail)
    {
        tail = tail->prev;
        if (tail) tail->next = nullptr;
    }
    else
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    return *current->disp;
}

double LinkedList::getConsumoAttuale(int currentTime) const
{
    if(isEmpty())
    {
        return 0;
    }

    double consumoTotale = 0;
    Node* current = head;
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

    Node* current = searchDispositivoName(nome);

    current->disp->setTimerOff();
}

void LinkedList::removeAllTimers()
{
    checkEmpty();

    Node* current = head;
    while(current)
    {
        current->disp->setTimerOff();
        current = current->next;
    }
}

double LinkedList::show(std::string nome) const
{
    checkEmpty();

    Node* current = searchDispositivoName(nome);
    return current->disp->calcolaConsumoEnergetico();
}

std::string LinkedList::showAll() const
{
    std::string stats = "[";
    Node* current = head;
    while(current)
    {
        stats += current->disp->showAllInfo() + ", ";
        current = current->next;
    }
    return stats+"]";
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
    return (head == nullptr);
}

std::ostream& operator<<(std::ostream& os, const LinkedList& list)
{
    if(list.isEmpty())
    {
        os << "Linked List is Empty!";
    }
    else 
    {
        LinkedList::Node* iteratorList = list.head;
        while(iteratorList != nullptr)
        {
            os << iteratorList->disp->getNome() << " ";
            iteratorList = iteratorList->next;
        }
    }

    return os;
}

void LinkedList::connectBefore(Node* p, Node* q)
{
    p->prev->next = q;
    q->prev = p->prev;

    q->next = p;
    p->prev = q;
}

void LinkedList::checkEmpty() const
{
    if(isEmpty())
    {
        throw std::out_of_range("Lista vuota!");
    }
}

LinkedList::Node* LinkedList::searchDispositivoName(const std::string nome) const
{
    checkEmpty();

    Node* current = head;
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

LinkedList::Node* LinkedList::searchDispositivoId(const int id) const
{
    checkEmpty();

    Node* current = head;
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

LinkedList::~LinkedList()
{
    Node* current = head;
    while(current)
    {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}
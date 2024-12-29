#include "../include/LinkedList.h"
using namespace std;

LinkedList::Node::Node(Dispositivo& data): prev{nullptr}, next{nullptr}, disp{&data} 
{ }

LinkedList::Node::Node(const Node& data): prev{data.prev}, next{data.next}, disp{data.disp}
{ }

LinkedList::Node& LinkedList::Node::operator=(const Node& data)
{
    prev = data.prev;
    next = data.next;
    disp = data.disp;
    return *this;
}

LinkedList::LinkedList(): head{nullptr}, tail{nullptr}
{ }

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

    if(current == head)      //significa che lo aggiungo all'inizio di tutti, quindi prima di head
    {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    else if(current == nullptr)  //significa che lo aggiungo alla fine di tutti, quindi dopo tail
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

void LinkedList::removeDispositivo(const string nome)
{
    if(isEmpty())
    {
        throw ListaVuotaException();
    }

    Node* current = searchDispositivo(nome);

    if(current == head)
    {
        head = head->next;
        head->prev = nullptr;
        delete current;
    }
    else if(current == tail)
    {
        tail = tail->prev;
        tail->next = nullptr;
        delete current;
    }
    else
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
    }
}

vector<Dispositivo*> LinkedList::removeAllDispositiviOff(const int currentTime)
{
    if(isEmpty())
    {
        throw ListaVuotaException();
    }

    vector<Dispositivo*> dispositiviSpenti;
    Node* current = head;
    while(current)
    {
        if(current->disp->getOrarioSpegnimento() <= currentTime)
        {
            dispositiviSpenti.push_back(current->disp);
            Node* temp = current;
            current = current->next;
            removeDispositivo(temp->disp->getNome());
        }
        else
        {
            current = current->next;
        }
    }

    return dispositiviSpenti;
}


void LinkedList::removeTimer(const string nome)
{
    if(isEmpty())
    {
        throw ListaVuotaException();
    }

    Node* current = searchDispositivo(nome);

    current->disp->setOrarioSpegnimento(Dispositivo::MAX_MINUTI_GIORNATA);
}

bool LinkedList::isEmpty() const
{
    return (head == nullptr);
}

ostream& operator<<(ostream& os, const LinkedList& list)
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

LinkedList::Node* LinkedList::searchDispositivo(const std::string nome) const
{
    if(isEmpty())
    {
        throw ListaVuotaException();
    }

    Node* current = head;
    while(current && current->disp->getNome() != nome)
    {
        current = current->next;
    }

    if(current == nullptr)
    {
        throw DispositivoNonTrovatoException();
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
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

void LinkedList::insert(Dispositivo& dispositivo)
{
    Node* newNode = new LinkedList::Node(dispositivo);
    if(isEmpty())
    {
        head = tail = newNode;
        return;
        cout << "In testa ho: " << head->disp->getNome() << endl;
    }

    //Trova il punto di inserimento
    Node* current = head;
    while(current && current->disp->getOrarioAccensione() < dispositivo.getOrarioAccensione())
    {
        current = current->next;
        if(current != nullptr)
        {
            cout << "INSERISCO " << newNode->disp->getNome() << " PRIMA DI " << current->disp->getNome() << endl;
        }
    }

    if(current == head)       //significa che l'orario di accensione del dispositivo che devo inserire viene prima del primo dispositivo che deve essere acceso e che sto mettendo l'oggetto all'inizio della coda
    {
        if(current != nullptr)
        {
            cout << "INSERISCO " << newNode->disp->getNome() << " PRIMA DI " << current->disp->getNome() << endl;
        }
        cout << " SONO DENTRO 1" << endl;
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    else if(current == nullptr)  //significa che lo aggiungo alla fine di tutti, quindi dopo tail
    {
        cout << " SONO DENTRO 2" << endl;
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    else
    {
        cout << " SONO DENTRO 3" << endl;
        connectBefore(current, newNode);
    }

    cout << "HEAD HA " << head->disp->getNome() << endl;
}

//Funzione che permette di collegare prima del nodo p il nodo q
void LinkedList::connectBefore(Node* p, Node* q)
{
    p->prev->next = q;
    q->prev = p->prev;

    q->next = p;
    p->prev = q;
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


LinkedList::~LinkedList()
{

}
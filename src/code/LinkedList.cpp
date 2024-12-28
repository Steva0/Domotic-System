#include "../header/LinkedList.h"
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
    Node* whereToInsert = head;
    Node* nodeToInsert = new LinkedList::Node(dispositivo);
    if(isEmpty())
    {
        head = nodeToInsert;
        tail = nodeToInsert;
        cout << "In testa ho: " << head->disp->getNome() << endl;
    }
    else
    {
        //Controllo di dove inserire nella posizione corretta dispositivo
        while(whereToInsert != nullptr && whereToInsert->disp->getOrarioAccensione() < dispositivo.getOrarioAccensione())
        {
            whereToInsert = whereToInsert->next;
            if(whereToInsert != nullptr)
            {
                cout << "INSERISCO " << nodeToInsert->disp->getNome() << " PRIMA DI " << whereToInsert->disp->getNome() << endl;
            }
        }

        if(whereToInsert == head)       //significa che l'orario di accensione del dispositivo che devo inserire viene prima del primo dispositivo che deve essere acceso e che sto mettendo l'oggetto all'inizio della coda
        {
            if(whereToInsert != nullptr)
            {
                cout << "INSERISCO " << nodeToInsert->disp->getNome() << " PRIMA DI " << whereToInsert->disp->getNome() << endl;
            }
            cout << " SONO DENTRO 1" << endl;
            nodeToInsert->next = head;
            head->prev = nodeToInsert;
            head = head->prev;
        }
        else if(whereToInsert == nullptr)  //significa che lo aggiungo alla fine di tutti, quindi dopo tail
        {
            cout << " SONO DENTRO 2" << endl;
            tail->next = nodeToInsert;
            nodeToInsert->prev = tail;
            tail = nodeToInsert;
        }
        else
        {
            cout << " SONO DENTRO 3" << endl;
            connectBefore(whereToInsert, nodeToInsert);
        }
    }
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
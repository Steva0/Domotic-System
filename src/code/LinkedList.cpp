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
    Node* nodeToInsert = new LinkedList::Node(dispositivo);
    if(isEmpty())
    {
        head = nodeToInsert;
        tail = nodeToInsert;
    }
    else
    {
        Node* insertPointer = head;

        //Controllo di dove inserire nella posizione corretta dispositivo
        while(insertPointer != nullptr && insertPointer->disp->getOrarioAccensione() < dispositivo.getOrarioAccensione())
        {
            insertPointer = insertPointer->next;
        }

        if(insertPointer == head)       //significa che l'orario di accensione del dispositivo che devo inserire viene prima del primo dispositivo che deve essere acceso e che sto mettendo l'oggetto all'inizio della coda
        {
            nodeToInsert->next = head;
            head->prev = nodeToInsert;
            head = nodeToInsert;
        }
        else if(insertPointer == nullptr)  //significa che lo aggiungo alla fine di tutti, quindi dopo tail
        {
            tail->next = nodeToInsert;
            nodeToInsert->prev = tail;
            tail = nodeToInsert;
        }
        else
        {
            connectNodes(insertPointer, nodeToInsert); //Se sono in mezzo, faccio il collegamento e non mi preoccupo di eventuali nullptr dato che mi trovo in mezzo alla lista
        }        

    }
}

void LinkedList::connectNodes(Node* before, Node* after)
{
    //Collega il nuovo nodo con quello successivo, se esiste
    before->next->prev = after; //after<-nodo successivo
    after->next = before->next; //after->nodo successivo
    
    //Collegamenti con before
    before->next = after;   //before->after
    after->prev = before;   //before<-after

}

bool LinkedList::isEmpty() const
{
    return (head == nullptr);
}

string LinkedList::toString() const
{
    Node* iteratorList = head;
    string nodesInList = "";
    while(iteratorList != nullptr)
    {
        nodesInList += iteratorList->disp->getNome();
        nodesInList += ", ";
        iteratorList = iteratorList->next;
    }

    return nodesInList;
}

ostream& operator<<(ostream& os, const LinkedList& list)
{
    if(list.isEmpty())
    {
        os << "Linked List is Empty!";
    }
    else 
    {
        os << list.toString();
    }

    return os;
}


LinkedList::~LinkedList()
{

}
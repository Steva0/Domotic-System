#include "../header/LinkedList.h"
using namespace std;

LinkedList::LinkedList(): head{nullptr}, tail{nullptr}
{ }

void LinkedList::insert(Dispositivo& dispositivo)
{
    Node* nodeToInsert = new Node(dispositivo);
    if(isEmpty())
    {
        head = nodeToInsert;
        tail = nodeToInsert;
    }
    else
    {
        Node* insertPointer = head;
        while(insertPointer->disp->getOrarioAccensione() < dispositivo.getOrarioAccensione())
        {
            insertPointer = insertPointer->next;
        }

        connectNodes(insertPointer, nodeToInsert);
        
        if(nodeToInsert->next == nullptr)
        {
            tail = nodeToInsert;
        }
    }
}

void LinkedList::connectNodes(Node* before, Node* after)
{
    //Collegamenti con after
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

Node* LinkedList::getHead() const
{
    return head;
}

ostream& operator<<(ostream& os, const LinkedList& list)
{
    if(list.isEmpty())
    {
        os << "Linked List is Empty!";
    }
    else 
    {
        Node* navigatePointer = list.getHead();
    }

    return os;
}


LinkedList::~LinkedList()
{

}
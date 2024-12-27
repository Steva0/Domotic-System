#include "../header/LinkedList.h"


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
            insertPointer = head->next;
        }

        connectNodes(insertPointer, nodeToInsert);
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

void LinkedList::setTail()
{
    while(tail->next != nullptr)
    {
        tail = tail->next;
    }
}

bool LinkedList::isEmpty()
{
    return (head == nullptr);
}

LinkedList::~LinkedList()
{

}
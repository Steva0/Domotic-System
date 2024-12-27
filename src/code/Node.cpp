//Alberto Bortoletto
#include "Node.h"

Node::Node(Dispositivo& data): prev{nullptr}, next{nullptr}, disp{&data} 
{ }

Node::Node(const Node& data): prev{data.prev}, next{data.next}, disp{data.disp}
{ }

Node& Node::operator=(const Node& data)
{
    prev = data.prev;
    next = data.next;
    disp = data.disp;
    return *this;
}

Dispositivo& Node::getDispositivo() const
{
    return *disp;
}

Node& Node::getNext() const
{
    return *next;
}

Node& Node::getPrev() const
{
    return *prev;
}

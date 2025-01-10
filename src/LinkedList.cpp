//Alberto Bortoletto

/*
L'idea di base e' la seguente:
Dato che i dispositivi che devono essere gestiti possono essere di tre tipi diversi (accesi / spenti / programmati) e che le operazioni che si possono fare su di essi sono simili, 
ho deciso di creare una classe base LinkedList che contiene tutte le funzioni comuni che possono essere svolte su una lista di dispositivi generica 
(indipendentemente da quale sia il tipo di dispositivi che quella lista contiene) e di realizzare tre classi derivate che rappresentano le tre tipologie di liste di dispositivi:
    - LinkedList: classe base che contiene tutte le funzioni comuni a tutte le liste
    - LinkedListOff: contiene tutti i dispositivi spenti
    - LinkedListOn: contiene tutti i dispositivi accesi
    - LinkedListProg: contiene tutti i dispositivi che si accenderanno in futuro

LinkedListOff e LinkedListOn "is a" LinkedList, LinkedListProg "is a" LinkedListOn

NB: Le classi sopracitate possono contenere dispositivi di qualsiasi tipo, tuttavia le funzioni membro definite per LinkedListOn, LinkedListOff e LinkedListProg sono specifiche e sensate per i dispositivi di tipo Dispositivo corrispondenti (accesi, spenti, programmati).

La classe LinkedList rappresenta una doubly LinkedList di Nodi realizzata tramite smart pointers.
Ogni oggetto LinkedList contiene:
    - HEAD: shared pointer al nodo in testa alla lista
    - TAIL: shared pointer al nodo in coda alla lista
        --> i puntatori condivisi sono necessari in quanto ci sono casi in cui il nodo head e tail puntano allo stesso nodo (lista con un solo nodo);

    - STATUS: una stringa che indica lo stato dei dispositivi nella lista

Ogni nodo contiene: 
    - DISPOSITIVO: un puntatore unico ad un Dispositivo
        --> il puntatore unico è necessario in quanto non è possibile avere più nodi che puntano allo stesso dispositivo

    - NEXT: shared pointer al nodo successivo
    - PREV: shared pointer al nodo precedente
        --> il puntatore condiviso è necessario in quanto ci sono casi in cui il nodo next di un nodo punta al nodo prev di un altro nodo; 

L'inserimento in LinkedList è ordinato in base all'orario di accensione del dispositivo con una politico FIFO, in modo tale che poi nella classe Interfaccia.cpp sia possibile accendere i dispositivi in ordine crescente di orario di accensione.
Tale inserimento e' comune a tutti i dispositivi eccetto che per la classe LinkedListOff che inserisce i dispositivi con una politicoa LIFO.

*/

#include "../include/LinkedList.h"

LinkedList::Node::Node(const Dispositivo& data): disp{std::make_unique<Dispositivo> (data)}, next{nullptr}, prev{nullptr}
{ }

//Crea una lista vuota impostando head e tail a nullptr
LinkedList::LinkedList(): head{nullptr}, tail{nullptr}
{ }

//Crea una lista inserendo un dispositivo in coda
LinkedList::LinkedList(Dispositivo& dispositivo): head{nullptr}, tail{nullptr}
{
    insert(dispositivo);
}

//Inserisce un dispositivo nella lista in modo ordinato in base all'orario di accensione secondo una politica LIFO
void LinkedList::insert(Dispositivo& dispositivo)
{
    std::shared_ptr<Node> newNode = std::make_shared<Node>(dispositivo);
    
    if(isEmpty())                       //Caso 1: Lista vuota
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

    if(current.get() == head.get())     //Caso 2: Inserimento in testa
    {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    else if(current == nullptr)         //Caso 3: Inserimento in coda
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    else                               //Caso 4: Inserimento in mezzo
    {
        current->prev->next = newNode;
        newNode->prev = current->prev;

        newNode->next = current;
        current->prev = newNode;
    }
}

Dispositivo LinkedList::removeDispositivoName(const std::string& nome)              //Rimuove il dispositivo con quel nome dalla lista e lo resituisce se esiste
{
    checkEmpty();

    return removeNode(searchDispositivoName(nome));
}

Dispositivo LinkedList::removeDispositivoId(const int id)
{
    checkEmpty();
    
    return removeNode(searchDispositivoId(id));
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

std::vector<Dispositivo> LinkedList::removeAll()
{
    try
    {
        checkEmpty();

        std::vector<Dispositivo> dispositiviSpenti;
        std::shared_ptr<Node> current = head;
        while(current)
        {
            std::shared_ptr<LinkedList::Node> prossimo = current->next;
            dispositiviSpenti.push_back(*(current->disp.get()));
            std::shared_ptr<Node> temp = current;
            removeDispositivoName(temp->disp->getNome());
            current = prossimo;
        }

        return dispositiviSpenti;
    }
    catch (const std::out_of_range& e)
    {
        return std::vector<Dispositivo>();
    }
}

void LinkedList::removeTimer(const std::string nome, const int currentTime)
{
    if(isEmpty())
    {
        return;
    }

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

void LinkedList::removeAllTimers(int currentTime)
{
    if(isEmpty())
    {
        return;
    }

    std::shared_ptr<Node> current = head;
    while(current)
    {
        removeTimer(current->disp->getNome(), currentTime);
        current = current->next;
    }
}

void LinkedList::resetAll()
{
    if(isEmpty())
    {
        return;
    }

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

    std::ostringstream statsStream;
    std::shared_ptr<Node> current = head;

    while (current)
    {
        if (current->disp->isGenerator())
        {
            statsStream << "Il dispositivo " << current->disp->getNome() << " ha prodotto ";
        }
        else
        {
            statsStream << "Il dispositivo " << current->disp->getNome() << " ha consumato ";
        }

        statsStream << std::fixed << std::setprecision(3) << std::fabs(current->disp->calcolaConsumoEnergetico()) << " kWh " << status;

        current = current->next;

        if(current)
        {
            statsStream << "\n\t";
        }
    }

    return statsStream.str();
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

void LinkedList::checkEmpty() const
{
    bool empty = isEmpty();
    if(empty)
    {
        throw std::out_of_range("Lista vuota!");
    }
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
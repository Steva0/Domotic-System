//Alberto Bortoletto

/*
L'idea di base e' la seguente:
Dato che i dispositivi che devono essere gestiti possono trovarsi in tre stati diversi (accesi / spenti / programmati) e che le operazioni che si possono fare su di essi sono simili, 
e' stato deciso di creare una classe base LinkedList che contiene tutte le funzioni comuni che possono essere svolte su una lista di dispositivi generica 
(indipendentemente da quale sia il tipo di dispositivi che quella lista contiene) e di realizzare tre classi derivate che rappresentano le tre tipologie di liste di dispositivi:
    - LinkedList: classe base che contiene tutte le funzioni comuni a tutte le liste
    - LinkedListOff: contiene tutti i dispositivi spenti
    - LinkedListOn: contiene tutti i dispositivi accesi
    - LinkedListProg: contiene tutti i dispositivi che si accenderanno in futuro

LinkedListOff e LinkedListOn "is a" LinkedList, LinkedListProg "is a" LinkedListOn

NB: Le classi sopracitate possono contenere dispositivi di qualsiasi tipo, tuttavia le funzioni membro definite per LinkedListOn, LinkedListOff e LinkedListProg 
    sono specifiche e sensate per i dispositivi di tipo Dispositivo corrispondenti (accesi, spenti, programmati).

La classe LinkedList rappresenta una doubly LinkedList di Nodi realizzata tramite smart pointers per permettere una corretta ed efficiente gestione della memoria.
Ogni oggetto di tipo LinkedList contiene:
    - HEAD: shared pointer al nodo in testa alla lista
    - TAIL: shared pointer al nodo in coda alla lista
        --> i shared pointer sono necessari in quanto ci sono casi in cui il nodo head e tail puntano allo stesso nodo (lista con un solo nodo), quindi non unique pointer;

    - STATUS: una stringa che indica lo stato dei dispositivi nella lista (acceso, spento, programmati)

Ogni nodo della lista contiene: 
    - DISPOSITIVO: un puntatore unico ad un Dispositivo
        --> il puntatore unico è necessario in quanto non vogliamo che un dispositivo sia condiviso tra più nodi;
        
    - NEXT: shared pointer al nodo successivo
    - PREV: shared pointer al nodo precedente
        --> il puntatore condiviso è necessario in quanto ci sono casi in cui il nodo next di un nodo punta al nodo prev di un altro nodo, quindi non unique pointer;; 

La politica adottata da LinkedList è del tipo FIFO.
Questa scelta e' stata fatta per 2 principali motivi:
    - EFFICIENZA: nel momento di rimozione di un nodo, tutti gli elementi alla destra di quel nodo non devono essere spostati uno ad uno (come nel caso di un vector), 
    bensi' basta modificare i puntatori next e prev del nodo precedente e successivo;
    - CASO BLACK OUT: nel caso in cui la potenza della casa non e' abbastanza per supportare tutti i dispositivi, si scorre tra quelli accesi e si spegne il primo che non e' sempre acceso. 
    Questo comportamento e' garantito dalla politica FIFO di una doubly LinkedList ed e' molto piu' efficiente rispetto che ad altre strutture come ad esempio un vector. 
    

Tale inserimento e' comune a tutti i dispositivi eccetto che per la classe LinkedListOff che inserisce i dispositivi con una politicoa LIFO.

NB: i distruttori di Node e LinkedList non sono stati appositamente implementati in quanto la memoria e' gia' gestita in modo corretto tramite l'utilizzo di smart pointers.

*/

#include "../include/LinkedList.h"

//Creazione di un nodo con un dispositivo al suo interno
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

//Rimuove il dispositivo con quel nome dalla lista e lo resituisce se esiste - Puo' lanciare un'eccezione throw std::out_of_range("Lista vuota!");
Dispositivo LinkedList::removeDispositivo(const std::string& nome)
{
    checkEmpty();

    return removeNode(searchDispositivo(nome));
}

//Rimuove il dispositivo con quell'id dalla lista e lo resituisce se esiste - Puo' lanciare un'eccezione throw std::out_of_range("Lista vuota!");
Dispositivo LinkedList::removeDispositivo(const int id)
{
    checkEmpty();
    
    return removeNode(searchDispositivo(id));
}

//Rimuove il primo dispositivo della lista in modo forzato e lo restituisce - Puo' lanciare un'eccezione throw std::out_of_range("Lista vuota!");
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

//Rimuove il primo dispositivo della lista che NON è sempre acceso e lo restituisce - Puo' lanciare un'eccezione throw std::out_of_range("Lista vuota!");
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

    return removeDispositivo(current->disp->getNome());
}

//Rimuove tutti i dispositivi dalla lista e li restituisce in un vector
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
            removeDispositivo(temp->disp->getNome());
            current = prossimo;
        }

        return dispositiviSpenti;
    }
    catch (const std::out_of_range& e)
    {
        return std::vector<Dispositivo>();
    }
}

//Rimuove il timer di un dispositivo
void LinkedList::removeTimer(const std::string nome, const int currentTime)
{
    if(isEmpty()) return;

    std::shared_ptr<Node> current = searchDispositivo(nome);

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

//Rimuove tutti i timer di tutti i dispositivi
void LinkedList::removeAllTimers(int currentTime)
{
    if(isEmpty()) return;

    std::shared_ptr<Node> current = head;
    while(current)
    {
        removeTimer(current->disp->getNome(), currentTime);
        current = current->next;
    }
}

//Ripristina l'orario di accensione e di spegnimento di tutti i dispositivi
void LinkedList::resetAll()
{
    if(isEmpty()) return;

    std::shared_ptr<Node> current = head;
    while(current)
    {
        current->disp->setOrarioAccensione(0);
        current->disp->setOrarioSpegnimento(0);
        current = current->next;
    }
}

//Controlla se un dispositivo con quel nome e' presente nella lista
bool LinkedList::contains(const std::string nome) const
{
    try
    {
        searchDispositivo(nome);
        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

//Controlla se un dispositivo con quell'id e' presente nella lista
bool LinkedList::contains(const int id) const
{
    try
    {
        searchDispositivo(id);
        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

//Controlla se la lista e' vuota
bool LinkedList::isEmpty() const
{
    return (head.get() == nullptr);
}

//Restituisce il consumo attuale di tutti i dispositivi accesi
double LinkedList::show(std::string nome) const
{
    checkEmpty();

    std::shared_ptr<Node> current = std::shared_ptr<Node>(searchDispositivo(nome));
    return current->disp->calcolaConsumoEnergetico();
}

//Restituisce una stringa contenente tutti i dispositivi presenti nella lista inLine
std::string LinkedList::showDevicesNames() const
{
    std::string stats = "[";
    std::shared_ptr<Node> current = head;
    while(current)
    {
        stats += current->disp->getNome() + " ";
        current = current->next;
    }
    return stats + "]";
}

//Restituisce una stringa contenente tutti i dispositivi presenti nella lista
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

//Restituisce una stringa contenente tutti i dispositivi presenti nella lista in modo dettagliato (principalmente per il debug)
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

//Cerca un dispositivo con quel nome nella lista e restituisce il nodo che lo contiene
std::shared_ptr<LinkedList::Node> LinkedList::searchDispositivo(const std::string nome) const
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

//Cerca un dispositivo con quell'ID nella lista e restituisce il nodo che lo contiene
std::shared_ptr<LinkedList::Node> LinkedList::searchDispositivo(const int id) const
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

//Rimuove un nodo dalla lista e restituisce il dispositivo contenuto
Dispositivo LinkedList::removeNode(std::shared_ptr<Node> current)
{
    if(current.get() == tail.get() && current.get() == head.get())          //Caso 1: Lista ha solo 1 nodo
    {
        head = tail = nullptr;
    }
    else if(current.get() == head.get())                                   //Caso 2: Rimozione in testa
    {   

        if(head->next) head = head->next;
        if(head->prev) head->prev->next = nullptr;
        head->prev = nullptr;
    } 
    else if(current.get() == tail.get())                                   //Caso 3: Rimozione in coda
    {
        if(tail->prev) tail = tail->prev;
        if(tail->prev) tail->next->prev = nullptr;
        tail->next = nullptr;
    }
    else                                                                    //Caso 4: Rimozione in mezzo
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    return *current->disp.get();
}

//Controlla se la lista e' vuota e lancia eccezione in caso in cui lo sia
void LinkedList::checkEmpty() const
{
    bool empty = isEmpty();
    if(empty)
    {
        throw std::out_of_range("Lista vuota!");
    }
}

//Operatore di output
//L'overloading di operator<< e' stato fatto chiamando una funzione membro inlinePrint() che restituisce una stringa contenente tutti i dispositivi presenti nella lista.
//Questo e' stato fatto in quanto operator<< e' stata dichiarata funzione helper della classe LinkedList e non funzione membro friend. 
std::ostream& operator<<(std::ostream& os, const LinkedList& list)
{
    if(list.isEmpty())
    {
        os << "Linked List is Empty!";
    }
    else 
    {
        os << list.showDevicesNames();
    }

    return os;
}
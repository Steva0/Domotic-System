//Alberto Bortoletto

/*
Descrizione Generale
La gestione dei dispositivi è stata implementata utilizzando una Doubly Linked List (Lista Doppiamente Collegata). 
Poiché i dispositivi possono essere in stati differenti (acceso, spento, programmato), sono state create tre classi aggiuntive oltre alla classe base LinkedList:

- LinkedList.h: Classe base che implementa le funzioni comuni a tutte le liste.
- LinkedListOn.h: Classe per gestire le funzioni specifiche dei dispositivi accesi.
- LinkedListOff.h: Classe per gestire le funzioni specifiche dei dispositivi spenti.
- LinkedListProg.h: Classe per gestire i dispositivi programmati (che si accenderanno in futuro).

Relazioni tra le classi:

LinkedListOff e LinkedListOn estendono la classe LinkedList (relazione "is a").
LinkedListProg estende LinkedListOn (relazione "is a").

Nota: Anche se queste classi possono contenere dispositivi di qualsiasi tipo, le funzioni membro di LinkedListOn, LinkedListOff e LinkedListProg sono progettate 
per essere utilizzate opportunamente con dispositivi che corrispondono ai rispettivi stati (accesi, spenti, programmati).

Implementazione della Classe Base LinkedList
La classe LinkedList rappresenta una lista doppiamente collegata di nodi, realizzata tramite smart pointers per garantire una gestione efficiente e sicura della memoria.

Ogni oggetto LinkedList contiene:

- head: Puntatore condiviso (shared pointer) al nodo in testa alla lista.
- tail: Puntatore condiviso (shared pointer) al nodo in coda alla lista.
Gli smart pointers condivisi sono usati per gestire casi in cui head e tail puntano allo stesso nodo (lista con un solo elemento).

- status: Stringa che indica lo stato dei dispositivi nella lista (ad esempio, "acceso", "spento", "programmato").

Ogni nodo della lista contiene:

- disp: Puntatore unico (unique pointer) a un oggetto Dispositivo per garantire che il dispositivo non venga condiviso tra più nodi.
- next: Puntatore condiviso (shared pointer) al nodo successivo.
- prev: Puntatore condiviso (shared pointer) al nodo precedente.

Politica di Inserimento: La classe utilizza una politica FIFO (First-In-First-Out) per la gestione dei nodi, con due principali vantaggi:

1) Efficienza: La rimozione di un nodo non richiede lo spostamento degli elementi successivi, come avviene con altre strutture (es. std::vector).
    Gestione Blackout: In caso di sovraccarico energetico, è possibile spegnere il primo dispositivo non critico con efficienza.

Nota: L'unica eccezione è la classe LinkedListOff, che adotta una politica LIFO (Last-In-First-Out) per inserire i dispositivi.

2) Gestione della Memoria: Non sono stati implementati distruttori espliciti per i nodi e la lista, poiché la memoria è gestita automaticamente tramite smart pointers.

Tale inserimento è comune a tutte le classi LinkedList eccetto che per la classe LinkedListOff che inserisce i dispositivi con una politica LIFO.

Funzionalità implementate

Inserimento ordinato:
- Basato sull'orario di accensione del dispositivo.
- Supporta i casi di lista vuota, inserimento in testa, coda o in mezzo.

Rimozione dispositivi:
- Rimozione per nome o ID.
- Rimozione forzata del primo dispositivo (es. gestione in caso di blackout).
- Rimozione di tutti i dispositivi con restituzione in un std::vector.

Gestione timer:
- Rimuove timer individuali o collettivi.
- Ripristina gli orari di accensione e spegnimento di tutti i dispositivi.

Ricerca e verifica:
- Verifica della presenza di un dispositivo tramite nome o ID.
- Restituzione del consumo totale di un dispositivo o della lista.

Debugging e visualizzazione:
- Funzioni per ottenere una rappresentazione dettagliata dei dispositivi nella lista.
- Overloading di operatori per stampa intuitiva.
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
    std::shared_ptr<Node> current = head;
    while(current != nullptr && current->disp->getOrarioAccensione() <= dispositivo.getOrarioAccensione())          //Trova il punto di inserimento
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

    //Se non ci sono dispositivi che non sono sempre accesi, rimuovo il primo dispositivo
    if(current == nullptr)                      
    {
        return forceRemoveFirst();
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
    
    current->disp->setHasTimer(false);        
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
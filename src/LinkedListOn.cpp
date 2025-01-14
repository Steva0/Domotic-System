//Alberto Bortoletto
#include "../include/LinkedListOn.h"

//Costruttore di default che chiama LinkedList() e modifica la stringa status
LinkedListOn::LinkedListOn(): LinkedList()
{ 
    status = "[ACCESO]";
}

//Costruttore con inserimento di un dispositivo in coda che chiama LinkedList(dispositivo) e modifica la stringa status
LinkedListOn::LinkedListOn(Dispositivo& dispositivo): LinkedList(dispositivo)
{ 
    status = "[ACCESO]";
}

//Rimuove tutti i dispositivi spenti (il cui orario di spegnimento e' prima dell'orario indicato) e li restituisce tutti in un vector - Puo' lanciare un'eccezione std::out_of_range("Lista vuota!")
std::vector<Dispositivo> LinkedListOn::removeDevicesToPowerOff(const int currentTime)
{
    checkEmpty();

    std::vector<Dispositivo> dispositiviSpenti;
    std::shared_ptr<Node> current = head;
    while(current)                                                          //Scorre tutta la lista
    {
        std::shared_ptr<Node> prossimo = current->next;
        if(!current->disp->isAcceso(currentTime))                           //Verifica se il dispositivo è spento
        {
            dispositiviSpenti.push_back(*(current->disp.get()));            //Inserisce il dispositivo spento nel vector
            std::shared_ptr<Node> temp = current;
            removeDispositivo(temp->disp->getNome());                       //Rimuove il nodo corrispondente al dispositivo spento
        }
        current = prossimo;
    }

    return dispositiviSpenti;
}

//Incrementa il tempo di tutti i dispositivi accesi in un minuto - Può lanciare eccezione std::invalid_argument e std::out_of_range
void LinkedListOn::incrementTimeOn()
{
    if(isEmpty()) return;

    std::shared_ptr<Node> current = head;
    while(current)
    {   
        current->disp->incrementaTempoAccensione(1);
        current = current->next;
    }
}

//Restituisce il consumo totale di tutti i dispositivi accesi
double LinkedListOn::getCurrentConsumption() const
{
    if(isEmpty())
    {
        return 0;
    }

    double consumoTotale = 0;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        consumoTotale += current->disp->getPotenza();

        current = current->next;
    }
    return consumoTotale;
}

//Restituisce l'energia prodotta in questo minuto
double LinkedListOn::producedEnergy() const 
{
    if(isEmpty())
    {
        return 0;
    }

    double energiaProdotta = 0;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        if(current->disp->isGenerator())
        {
            energiaProdotta += current->disp->getPotenza();
        }
        current = current->next;
    }

    //Prendo il il valore assoluto dell'energia prodotta
    energiaProdotta = energiaProdotta < 0 ? -1*energiaProdotta : energiaProdotta;

    return energiaProdotta/60;
}

//Restituisce il modulo dell'energia consumata in questo minuto
double LinkedListOn::consumedEnergy() const 
{
    if(isEmpty())
    {
        return 0;
    }

    double energiaUtilizzata = 0;
    std::shared_ptr<Node> current = head;
    while(current)
    {
        if(!current->disp->isGenerator())
        {
            energiaUtilizzata += current->disp->getPotenza();
        }
        current = current->next;
    }

    //Prendo il il valore assoluto dell'energia utilizzata
    energiaUtilizzata = energiaUtilizzata < 0 ? -1*energiaUtilizzata : energiaUtilizzata;

    return energiaUtilizzata/60;
}
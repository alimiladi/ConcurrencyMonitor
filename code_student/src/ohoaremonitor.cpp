#include "ohoaremonitor.h"
#include <QThread>

OHoareMonitor::Condition::Condition():
    condition(){}

HoareMonitor::Condition *OHoareMonitor::Condition::getCondition()
{
    return &condition;
}

void OHoareMonitor::Condition::setName(QString name)
{
    this->name = name;
}

QString OHoareMonitor::Condition::getName()
{
    return name;
}

OHoareMonitor::OHoareMonitor():
    hoareMonitor(),
    mutex(QMutex::NonRecursive),
    nb_waiting(0),
    lockedMutex(0){}

void OHoareMonitor::monitorIn()
{
    //Simple réutilisation de la méthode monitorIn de la classe HoareMonitor
    hoareMonitor.monitorIn();
}

void OHoareMonitor::monitorOut()
{
    //Simple réutilisation de la méthode monitorOut de la classe HoareMonitor
    hoareMonitor.monitorOut();
}


void OHoareMonitor::wait(Condition &condition)
{
    mutex.lock();
    //Incrément du nombre de threads en attente sur la variable de condition
    nb_waiting ++;
    //Rajout du nom du thread appelant cette méthode dans la liste d'attente
    // de la variable de condition
    ReadWriteLogger::getInstance()->addWaiting(condition.getName());
    mutex.unlock();
    //Réutilisation de la méthode wait de HoareMonitor
    hoareMonitor.wait(*(condition.getCondition()));
    mutex.lock();
    //Décrément du nombre de threads en attente sur cette variable condition
    // dès le réveil du wait par un signal
    nb_waiting--;
    //Enlèvement du nom du thread appelant cette méthode de la queue d'attente
    // de la variable condition
    ReadWriteLogger::getInstance()->removeWaiting(condition.getName());
    mutex.unlock();
    //Blocage au niveau du mutex vérouillé si au moins un thread a appelé cette
    // méthode
    if(nb_waiting > 0)
        lockedMutex.acquire();
}


void OHoareMonitor::signal(Condition &condition)
{
    mutex.lock();
    // Comme il s'agit d'un moniteur de Hoare, la méthode signal est bloquante
    // car n'importe quel thread va essyer de donner la main à quelqu'un d'autre
    // en attente et bloquer
    if(nb_waiting > 0){
        //Incrément du nombre de threads en attente
        nb_waiting ++;
        //Rajout du nom du thread dans la liste d'attente de la variable condition
        ReadWriteLogger::getInstance()->addWaiting(condition.getName());
        //Libération du mutex vérouillé
        lockedMutex.release();
    }
    mutex.unlock();
    //Dans tous les cas (blocante ou non), cette méthode va réutiliser la méthode
    // signal de HoareMonitor
    hoareMonitor.signal(*(condition.getCondition()));
}


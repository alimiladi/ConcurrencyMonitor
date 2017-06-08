#ifndef OHOAREMONITOR_H
#define OHOAREMONITOR_H

#include "owaitcondition.h"
#include "hoaremonitor.h"


class OHoareMonitor
{
protected:

    class Condition{
    public:
        Condition();
        HoareMonitor::Condition *getCondition();

    private:
        HoareMonitor::Condition condition;
    };

public:
    OHoareMonitor();

    /**
     * This function has to be called at the beginning of each function being
     * an entry point to the monitor.
     */
    void monitorIn();

    /**
     * This function has to be called at the end of each function being
     * an entry point to the monitor.
     */
    void monitorOut();

    /**
     * This function implements the waiting on a condition, as defined by Hoare.
     * When the thread is waken by a signal, it continues with the mutual
     * exclusion.
     */
    void wait(Condition &cond);

    /**
     * This function implements the signaling of a condition, as defined by
     * Hoare. If no thread is waiting for the condition, then nothing happens,
     * but if there is one the thread calling signal is suspended, waiting for
     * the other one to finish.
     */
    void signal(Condition &cond);

    void setName(QString name){

    }


private:

    HoareMonitor hoareMonitor;
    QString name;
};

#endif // OHOAREMONITOR_H

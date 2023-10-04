#ifndef SchedulerChoice_H
#define SchedulerChoice_H


#include <iostream>
#include "schedulers/SJF.h"
#include "schedulers/FCFS.h"
#include "schedulers/priority.h"
#include "schedulers/priorityNoPreemption.h"
#include "schedulers/roundRobin.h"
#include "schedulers/scheduler.h"
#include "cpu.h"
#include "metrics.h"

class SchedulerChoice {
public:
    static void displayMenu();

    int getUserChoice();

    Scheduler* handleSchedulerChoice(CPU *cpu, Time *time, Metrics *Metrics, std::queue<Process*>& processToScheduler);

private:
    int choice;
};

#endif // SchedulerChoice_H
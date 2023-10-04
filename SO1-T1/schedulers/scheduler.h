#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../process.h"

class Scheduler {
public:
    virtual void addProcess(std::unique_ptr<Process> process) = 0;
    virtual bool run() = 0;
    virtual void dispatcher() = 0;
    virtual void yield() = 0;
    virtual void init() = 0;
    
private:

};

#endif

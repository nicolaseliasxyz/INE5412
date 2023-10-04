#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include <vector>
#include <queue>
#include <memory>
#include "../process.h"
#include "../time.h"
#include "scheduler.h"
#include "../cpu.h"
#include "../metrics.h"


class PRIORITY: public Scheduler {
public:
    PRIORITY(CPU& cpu, Time& time, Metrics& metrics, std::queue<Process*>& processToScheduler)
    : cpu(cpu), time(time), processToScheduler(processToScheduler), metrics(metrics) {}

    void addProcess(std::unique_ptr<Process> process) override;
    bool run() override;
    void yield() override;
    void dispatcher() override;
    void init() override;
    
    static bool compareByPriority(const std::unique_ptr<Process>& a, const std::unique_ptr<Process>& b);

    void interruptByPriority();
    
private:
    CPU& cpu;
    Time& time;
    Metrics& metrics;
    int contextSwapCounter = 0;
    std::unique_ptr<Process> currentProcess;
    std::vector<std::unique_ptr<Process>> processVector;
    std::queue<Process*>& processToScheduler;
    std::vector<Process> processEnded;
};

#endif
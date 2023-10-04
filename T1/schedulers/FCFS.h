#ifndef FCFS_H
#define FCFS_H

#include <queue>
#include <memory>
#include "../process.h"
#include "scheduler.h"
#include "../cpu.h"
#include "../time.h"
#include "../metrics.h"

class FCFS : public Scheduler {
public:
    FCFS(CPU& cpu, Time& time, Metrics& metrics, std::queue<Process*>& processToScheduler)
    : cpu(cpu), time(time), processToScheduler(processToScheduler), metrics(metrics) {}
    void addProcess(std::unique_ptr<Process> process) override;
    bool run() override;
    void yield() override;
    void dispatcher() override;
    void init() override;
    
private:
    CPU& cpu;
    Time& time;
    Metrics& metrics;
    int contextSwapCounter = 0;
    std::unique_ptr<Process> currentProcess;
    std::queue<std::unique_ptr<Process>> processQueue;
    std::queue<Process*>& processToScheduler;
    std::vector<Process> processEnded;
};

#endif

#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include <queue>
#include <memory>
#include "../process.h"
#include "../cpu.h"
#include "../time.h"
#include "../metrics.h"
#include "scheduler.h"

class ROUND_ROBIN : public Scheduler {
public:
    ROUND_ROBIN(CPU& cpu, Time& time, Metrics& metrics, std::queue<Process*>& processToScheduler)
    : cpu(cpu), time(time), processToScheduler(processToScheduler), metrics(metrics), quantum(2) {}
    
    
    void addProcess(std::unique_ptr<Process> process) override;
    bool run() override;
    void yield() override;
    void dispatcher() override;
    void init() override;
    
    void interruptByQuatum();
    
private:
    CPU& cpu;
    Time& time;
    Metrics& metrics;
    int quantum;
    int contextSwapCounter = 0;
    std::unique_ptr<Process> currentProcess;
    std::queue<std::unique_ptr<Process>> processQueue;
    std::queue<Process*>& processToScheduler;
    std::vector<Process> processEnded;
};

#endif //

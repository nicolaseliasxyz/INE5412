#ifndef SJF_SCHEDULER_H
#define SJF_SCHEDULER_H

#include <vector>
#include <queue>
#include <memory>
#include "../process.h"
#include "../cpu.h"
#include "scheduler.h"
#include "../time.h"
#include "../metrics.h"

class SJF: public Scheduler {
public:
    SJF(CPU& cpu, Time& time, Metrics& metrics, std::queue<Process*>& processToScheduler)
    : cpu(cpu), time(time), processToScheduler(processToScheduler), metrics(metrics) {}
    
    void addProcess(std::unique_ptr<Process> process) override;
    bool run() override;
    void yield() override;
    void dispatcher() override;
    void init() override;
    
    static bool compareByShorterDuration(const std::unique_ptr<Process>& a, const std::unique_ptr<Process>& b);
    
private:
    std::unique_ptr<Process> currentProcess;
    CPU& cpu;
    Time& time;
    Metrics& metrics;
    int contextSwapCounter = 0;
    std::vector<std::unique_ptr<Process>> processVector;
    std::queue<Process*>& processToScheduler;
    std::vector<Process> processEnded;
};

#endif
#ifndef METRICS_H
#define METRICS_H

#include <iostream>
#include <vector>
#include "process.h"
#include "time.h"

class Metrics {
public:
    Metrics() {}

    void makeMetrics(std::vector<Process>* processVector, int contextSwapCounter, Time* time);

    void turnaroundTimePerProcess();
    void turnaroundTimeAverage();
    void totalContextSwaps(int totalContextSwaps);
    void waitingTimePerProcess();
    void waitingTimeAverage();
    void executionDiagram(Time* time);

    static bool compareByPid(const Process& a, const Process& b);

private:
    std::vector<Process> processVector;
};


#endif // METRICS_H

#include "metrics.h"
#include <algorithm>
#include <iostream>

void Metrics::makeMetrics(std::vector<Process>* processVector, int contextSwapCounter, Time* time) {
    std::cout << "\n-----> METRICS FOR THIS SIMULATION: <-----" << std::endl;
    this->processVector = *processVector;
    // Remover o primeiro ready status errado do init
    this->processVector[0].statusQueue.pop();
    turnaroundTimePerProcess();
    turnaroundTimeAverage();
    totalContextSwaps(contextSwapCounter);
    waitingTimePerProcess();
    executionDiagram(time);
}

void Metrics::turnaroundTimePerProcess() {
    std::cout << "\n-----> TURNAROUND TIME PER PROCESS: <-----" << std::endl;
    for (int i = 0; i < this->processVector.size(); i++) {
        double turnaround = abs(processVector[i].getFinalTime() - processVector[i].getCreationTime());
        std::cout << "Process " << processVector[i].getPid () << " Turnaround Time: " << turnaround << std::endl;
    }
}

void Metrics::turnaroundTimeAverage() {
    std::cout << "\n-----> TURNAROUND TIME AVERAGE: <-----" << std::endl;
    double turnaroundMiddle = 0;
    for (int i = 0; i < this->processVector.size(); i++) {
        turnaroundMiddle += abs(processVector[i].getFinalTime() - processVector[i].getCreationTime());
    }
    turnaroundMiddle = turnaroundMiddle / this->processVector.size();
    std::cout << "Turnaround Time Average: " << turnaroundMiddle << std::endl;
}

void Metrics::totalContextSwaps(int contextSwapCounter) {
    std::cout << "\n-----> TOTAL CONTEXT SWAPS: <-----" << std::endl;
    std::cout << "Total Context Swaps: " << contextSwapCounter << std::endl;
}

void Metrics::waitingTimePerProcess() {
    std::cout << "\n-----> WAITING TIME PER PROCESS: <-----" << std::endl;
    for (int i = 0; i < this->processVector.size(); i++) {
        double waitingTime = abs(processVector[i].getFinalTime() - processVector[i].getCreationTime() - processVector[i].getDuration());
        std::cout << "Process " << processVector[i].getPid () << " Waiting Time: " << waitingTime << std::endl;
    }
}

void Metrics::waitingTimeAverage() {
    std::cout << "\n-----> WAITING TIME AVERAGE: <-----" << std::endl;
    double waitingTimeMiddle = 0;
    for (int i = 0; i < this->processVector.size(); i++) {
        waitingTimeMiddle += abs(processVector[i].getFinalTime() - processVector[i].getCreationTime() - processVector[i].getDuration());
    }
    waitingTimeMiddle = waitingTimeMiddle / this->processVector.size();
    std::cout << "Waiting Time Average: " << waitingTimeMiddle << std::endl;
}

void Metrics::executionDiagram(Time* time) {
    std::sort(this->processVector.begin(), this->processVector.end(), compareByPid);
    std::cout << "\n-----> EXECUTION DIAGRAM: <-----" << std::endl;
    std::string topString = "\ntime    ";
    
    for (int i = 0; i < this->processVector.size(); i++) {
        topString += "P" + std::to_string(processVector[i].getPid()) + "  ";
    }
    std::cout << topString << std::endl;
    
    std::string bottomStringProcess;
    std::queue<std::tuple<Process::State, int>> statusProcess = processVector[0].statusQueue;
    std::string interval = std::to_string(0) + "-" + std::to_string(1) + "     ";
    
    int numProcesses = processVector.size();

    for (int i = 0; i < time->globalTime; ++i) {
    std::string interval = std::to_string(i) + "-" + std::to_string(i + 1) + "   ";
    std::string bottomStringProcess;

    if (i < 10) {
        interval = "  " + interval;
    }

    for (int j = 0; j < numProcesses; ++j) {
        std::queue<std::tuple<Process::State, int>>& statusProcess = processVector[j].statusQueue;
        
        if (!statusProcess.empty() && std::get<1>(statusProcess.front()) == i) {
            if (std::get<0>(statusProcess.front()) == Process::State::RUNNING) {
                bottomStringProcess += "##  ";
            } else if (std::get<0>(statusProcess.front()) == Process::State::READY) {
                bottomStringProcess += "--  ";
            } else {
                bottomStringProcess += "    ";
            }
            statusProcess.pop();
        } else {
            bottomStringProcess += "    ";
        }
    }
    std::cout << interval + bottomStringProcess << std::endl;
    }    
}

bool Metrics::compareByPid(const Process& a, const Process& b) {
    return a.getPid() < b.getPid();
}

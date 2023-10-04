#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <memory>
#include <queue>
#include "cpu.h"

class Process {
public:
    Process(int creationTime, int duration, int priority, int pid, CPU::Context* context);
    enum class State { NEW, RUNNING, READY, FINISHED};
    
    int getCreationTime() const;
    int getTimeInReady() const;
    int getFinalTime() const;
    int getTimeInExecution() const;
    int getContextSwapCounter() const;

    int getRemainingTime() const;
    std::string getLineTimeInExecution();

    int getPid() const;
    int getDuration() const;
    int getPriority() const;
    int getQuantumAcumulator() const;

    Process::State getState();

    CPU::Context* getContext();

    void setTimeInExecution(int time);
    void setFinalTime(int time);
    void setState(State newState, int time);
    
    bool execute();
    bool itsDone() const;

    std::queue<std::tuple<State, int>> statusQueue;
private:
    CPU::Context* _context;
    State _state = State::NEW;

    int _timeInExecution;

    int _pid;

    int _contextSwapCounter;

    int _quantumAcumulator = 0;

    int _finalTime;
    int _creationTime;

    int _duration;
    int _priority;
};

#endif

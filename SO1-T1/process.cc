#include "process.h"

Process::Process(int creationTime, int duration, int priority, int pid, CPU::Context* context)
    : _context(context), _state(State::READY), _pid(pid), _creationTime(creationTime), _duration(duration), _priority(priority)
{
    _contextSwapCounter = 0;
    _timeInExecution = 0;
    _finalTime = 0;
}

int Process::getPid() const {
    return _pid;
}

int Process::getCreationTime() const {
    return _creationTime;
}

int Process::getFinalTime() const {
    return _finalTime;
}

int Process::getDuration() const {
    return _duration;
}

int Process::getPriority() const {
    return _priority;
}

int Process::getTimeInExecution() const {
    return _timeInExecution;
}

void Process::setState(State newState, int time) {
    std::tuple<State, int> stateHistoric(newState, time);
    statusQueue.push(stateHistoric);
    _state = newState;
}

Process::State Process::getState() {
    return _state;
}

CPU::Context* Process::getContext() {
    return _context;
}

bool Process::itsDone() const {
    if (_duration - _timeInExecution > 0) {
        return false;
    } else {
        return true;
    }
}

bool Process::execute() {
    _timeInExecution++;
    return itsDone();
}

void Process::setTimeInExecution(int time) {
    _timeInExecution = time;
}

void Process::setFinalTime(int time) {
    _finalTime = time;
}

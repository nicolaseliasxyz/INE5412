#include "priorityNoPreemption.h"
#include <iostream>
#include <algorithm>

void PRIORITY_NO_PREEMPTION::addProcess(std::unique_ptr<Process> process) {
    process->setState(Process::State::READY, time.globalTime);
    processVector.push_back(std::move(process));
    std::sort(processVector.begin(), processVector.end(), compareByPriority);
}

void PRIORITY_NO_PREEMPTION::yield() {
    if (currentProcess->itsDone()) {
        currentProcess->setState(Process::State::FINISHED, time.globalTime);
        currentProcess->setFinalTime(time.globalTime);
        std::cout << "Processo ID: " << currentProcess->getPid() << ", finalizado." << std::endl;
        // Criar copia do processo para metricas
        Process copyOfCurrentProcess(*currentProcess);
        processEnded.push_back(copyOfCurrentProcess);
    }

    if (processVector.empty()) {
        currentProcess = nullptr;
        return;
    }
    std::unique_ptr<Process> prev = std::move(currentProcess);
    std::unique_ptr<Process> next = std::move(processVector[0]);
    processVector.erase(processVector.begin());
    
    cpu.switch_context(prev->getContext(), next->getContext());
    next->setState(Process::State::RUNNING, time.globalTime);
    currentProcess = std::move(next);
    contextSwapCounter++;
    
    if (prev->getState() != Process::State::FINISHED && prev->getState() != Process::State::READY) {
        this->addProcess(std::move(prev));
    }
}

void PRIORITY_NO_PREEMPTION::dispatcher() {
    bool noMoreActons = false;
    while (!noMoreActons) {
        // Suponho que o arquivo de entrada esta estruturado em oderm crescente de tempo de criacao
        if (!processToScheduler.empty() && processToScheduler.front()->getCreationTime() == time.globalTime) {
            std::unique_ptr<Process> processPtr = std::make_unique<Process>(*processToScheduler.front());
            addProcess(std::move(processPtr));
            processToScheduler.pop();
        } else {
            noMoreActons = true;
        }
    }  
}

bool PRIORITY_NO_PREEMPTION::run() {
    while (currentProcess != nullptr) {
        std::cout << "Processo ID: " << currentProcess->getPid() << ", em execução." << std::endl;

        bool finished = false;
        while (!finished) {
            cpu.execute();
            finished = currentProcess->execute();
            time.incrementGlobalTime();
            dispatcher();
        }
        yield();
    }
    std::cout << "Todos os processos foram finalizados." << std::endl;
    metrics.makeMetrics(&processEnded, contextSwapCounter, &time);
    return true;
}

bool PRIORITY_NO_PREEMPTION::compareByPriority(const std::unique_ptr<Process>& a, const std::unique_ptr<Process>& b) {
    return a->getPriority() > b->getPriority();
}

void PRIORITY_NO_PREEMPTION::init() {
    std::cout << "Escalonamento PRIORITY_NO_PREEMPTIVE:" << std::endl;
    dispatcher();
    currentProcess = std::move(processVector[0]);
    currentProcess->setState(Process::State::RUNNING, time.globalTime);
    processVector.erase(processVector.begin());
    // Certo seria ter uma troca de contexto do main com o processo inicial
    // Computei uma troca aqui por conta do enunciado
    contextSwapCounter++;
    run();
}

#include <iostream>
#include "roundRobin.h"

void ROUND_ROBIN::addProcess(std::unique_ptr<Process> process) {
    process->setState(Process::State::READY, time.globalTime);
    processQueue.push(std::move(process));
}

void ROUND_ROBIN::yield() {
   if (currentProcess->itsDone()) {
        currentProcess->setState(Process::State::FINISHED, time.globalTime);
        currentProcess->setFinalTime(time.globalTime);
        std::cout << "Processo ID: " << currentProcess->getPid() << ", finalizado." << std::endl;
        // Criar copia do processo para metricas
        Process copyOfCurrentProcess(*currentProcess);
        processEnded.push_back(copyOfCurrentProcess);
    }

    if (processQueue.empty()) {
        currentProcess = nullptr;
        return;
    }
    std::unique_ptr<Process> prev = std::move(currentProcess);
    std::unique_ptr<Process> next = std::move(processQueue.front());
    processQueue.pop();
    
    cpu.switch_context(prev->getContext(), next->getContext());
    next->setState(Process::State::RUNNING, time.globalTime);
    currentProcess = std::move(next);
    contextSwapCounter++;

    if (prev->getState() != Process::State::FINISHED && prev->getState() != Process::State::READY) {
        this->addProcess(std::move(prev));
    }
}

bool ROUND_ROBIN::run() {
    while (currentProcess != nullptr) {
        std::cout << "Processo ID: " << currentProcess->getPid() << ", em execução." << std::endl;
        bool finished = false;
        while (!finished) {
            cpu.execute();
            finished = currentProcess->execute();
            time.incrementGlobalTime();
            dispatcher();
            interruptByQuatum();
        }
        yield();
    }
    std::cout << "Todos os processos foram finalizados." << std::endl;
    metrics.makeMetrics(&processEnded, contextSwapCounter, &time);
    return true;
}

void ROUND_ROBIN::dispatcher() {
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

void ROUND_ROBIN::interruptByQuatum() {
    if (currentProcess->getTimeInExecution() == quantum) {
        yield();
    }
}

void ROUND_ROBIN::init() {
    std::cout << "Escalonamento ROUND_ROBIN:" << std::endl;
    dispatcher();
    currentProcess = std::move(processQueue.front());
    currentProcess->setState(Process::State::RUNNING, time.globalTime);
    processQueue.pop();
    // Certo seria ter uma troca de contexto do main com o processo inicial
    // Computei uma troca aqui por conta do enunciado
    contextSwapCounter++;
    run();
}

#include <iostream>
#include "schedulerChoise.h"

void SchedulerChoice::displayMenu() {
    std::cout << "Choose the Scheduler:" << std::endl;
    std::cout << "1. SJF: Shortest Job First" << std::endl;
    std::cout << "2. RR: Round Robin" << std::endl;
    std::cout << "3. PRIORITY NON-PREEMPTIVE" << std::endl;
    std::cout << "4. PRIORITY PREEMPTIVE" << std::endl;
    std::cout << "5. FCFS: First Come First Served" << std::endl;
}

int SchedulerChoice::getUserChoice() {
    std::cout << "Enter your choice (1-5): ";
    std::cin >> choice;
    return choice;
}

Scheduler *SchedulerChoice::handleSchedulerChoice(CPU *cpu, Time *time, Metrics *metrics, std::queue<Process*>& processToScheduler) {
    SchedulerChoice scheduler{};
    SchedulerChoice::displayMenu();

    int userChoice = scheduler.getUserChoice();

    switch (userChoice) {
        case 1:
            std::cout << "You chose SJF: Shortest Job First" << std::endl;
            return new SJF(*cpu, *time, *metrics, processToScheduler);
        case 2:
            std::cout << "You chose RR: Round Robin" << std::endl;
            return new ROUND_ROBIN(*cpu, *time, *metrics, processToScheduler);
        case 3:
            std::cout << "You chose PRIORITY NON-PREEMPTIVE" << std::endl;
            return new PRIORITY_NO_PREEMPTION(*cpu, *time, *metrics, processToScheduler);
        case 4:
            std::cout << "You chose PRIORITY PREEMPTIVE" << std::endl;
            return new PRIORITY(*cpu, *time, *metrics, processToScheduler);
        case 5:
            std::cout << "You chose FCFS: First Come First Served" << std::endl;
            return new FCFS(*cpu, *time, *metrics, processToScheduler);
        default:
            std::cout << "Invalid choice. Please enter a number between 1 and 5." << std::endl;
            return nullptr;
    }
}

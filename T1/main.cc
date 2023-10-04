#include <memory>
#include <queue>
#include "read_file.cc"
#include "process.h"
#include "cpu.h"
#include "schedulerChoise.h"
#include "time.h"
#include "metrics.h"

int main()
{	
	Time t;
	File f;
	CPU cpu;
	Metrics metrics;
	f.read_file();
	
	std::queue<Process*> processToScheduler;
	int i = 1;
	while (!f.processes.empty()) {
		CPU::Context* context = new CPU::Context(cpu);
		Process* processPtr = new Process(
			f.processes.front()->creation_time,
			f.processes.front()->duration,
			f.processes.front()->priority,
			i++,
			context
		);
		processToScheduler.push(processPtr);
		f.processes.erase(f.processes.begin());
	}

	SchedulerChoice schedulerChoice;
    Scheduler* scheduler = schedulerChoice.handleSchedulerChoice(&cpu, &t, &metrics, processToScheduler);
	if (scheduler == nullptr) {
		delete scheduler;
		return 0;
	}

	// Inicia a simulação
	scheduler->init();

	delete scheduler;
	return 0;
}

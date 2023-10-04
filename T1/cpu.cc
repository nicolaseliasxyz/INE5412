#include "cpu.h"
#include <iostream>
#include <random>

CPU::CPU() {
    _programCounter = 0;
    _stackPointer = 0;
    _statusRegister = 0;
    for (int i = 0; i < NUM_REGISTERS; i++) {
        _regs[i] = 0;
    }
}

void CPU::Context::save()
{   
    for (int i = 0; i < NUM_REGISTERS; i++) {
        regs[i] = cpu._regs[i];
    }

    stackPointer = cpu._stackPointer;
    programCounter = cpu._programCounter;
    statusRegister = cpu._statusRegister;
}

void CPU::Context::load()
{
    for (int i = 0; i < NUM_REGISTERS; i++) {
        cpu._regs[i] = regs[i];
    }

    cpu._stackPointer = stackPointer;
    cpu._programCounter = programCounter;
    cpu._statusRegister = statusRegister;
}

CPU::Context::~Context()
{
    if (_stack)
        delete(_stack);
}

void CPU::switch_context(Context* from, Context* to)
{
    from->save();
    to->load();
}

void CPU::execute()
{
    _programCounter++;
    _stackPointer = static_cast<uint64_t>(random());
    _statusRegister = static_cast<uint64_t>(random());

    for (int i = 0; i < NUM_REGISTERS; i++) {
        _regs[i] = static_cast<uint64_t>(random());
    }
}

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <stack>
#include <memory>

class CPU {
public:
    static const int NUM_REGISTERS = 6;
    CPU();
    class Context {
    public:
        static const int NUM_REGISTERS = 6;

        Context(CPU& cpu) : cpu(cpu) {}
        uint64_t regs[NUM_REGISTERS];
        uint64_t stackPointer;
        uint64_t programCounter;
        uint64_t statusRegister;

        void save();
        void load();
        ~Context();
    private:
        CPU& cpu;
        uint64_t* _stack;
    };
   
    void switch_context(Context* from, Context* to);
    void execute();
private:
    uint64_t _stackPointer;
    uint64_t _programCounter;
    uint64_t _statusRegister;
    uint64_t _regs[NUM_REGISTERS];
    uint64_t _stack;
};



#endif // CPU_H

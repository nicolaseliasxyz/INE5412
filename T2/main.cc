#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "PageReplacementAlgorithms/FIFO.h"
#include "PageReplacementAlgorithms/LRU.h"
#include "PageReplacementAlgorithms/OPT.h"
#include "Calculator.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: ./simulador <número de quadros> < <path arquivo de entrada>" << std::endl;
        return 1;
    }

    int frameSize = std::atoi(argv[1]);
    // Lê as referencias.
    std::vector<int> refs;
    int page;
    while (!std::cin.eof()) {
        if (std::cin >> page) {
            refs.push_back(page);
        }
    }

    RAM ram(frameSize);

    FIFO fifoInstance(ram);
    LRU lruInstance(ram);
    OPT optInstance(ram, refs);
    
    std::vector<PageReplacementAlgorithm*> algorithms = {&fifoInstance, &lruInstance, &optInstance};
    auto refsSize = refs.size();
    // Executa os algoritmos.
    for (PageReplacementAlgorithm* algorithm : algorithms) {
        std::vector<int> refs_copy = refs;
        while (!refs_copy.empty()) {
            algorithm->referencePage(refs_copy.front());            
            refs_copy.erase(refs_copy.begin());
        }
        ram.reset();
    }

    // Exibe os resultados.
    Calculator calculator{};
    calculator.showResults(algorithms, frameSize, refsSize);

    return 0;
}

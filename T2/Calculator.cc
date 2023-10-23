#include "Calculator.h"
#include <iostream>

void Calculator::showResults(std::vector<PageReplacementAlgorithm*> algorithms, int frameSize, size_t refSize) {
    std::cout << frameSize << " quadros" << std::endl;
    std::cout << refSize << " refs" << std::endl;

    for (size_t i = 0; i < algorithms.size(); ++i) {
        std::string algoName = algorithms.at(i)->getName();

        int pfCount = algorithms.at(i)->getPageFaults();

        std::cout << algoName << ": " << pfCount << " PFs" << std::endl;
    }
}

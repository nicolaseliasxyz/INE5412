#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "PageReplacementAlgorithms/PageReplacementAlgorithm.h"
#include <vector>

class Calculator {
public:
    Calculator() = default;
    void showResults(std::vector<PageReplacementAlgorithm*> algorithms, int frameSize, size_t refSize);
};

#endif //CALCULATOR_H

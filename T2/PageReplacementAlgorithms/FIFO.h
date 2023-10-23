#ifndef FIFO_H
#define FIFO_H

#include "PageReplacementAlgorithm.h"
#include <queue>

class FIFO : public PageReplacementAlgorithm {
public:
    explicit FIFO(RAM& ram) : PageReplacementAlgorithm(ram) {}

    void referencePage(int page) override;
    
    bool search(int key);

    std::string getName() const override { return "FIFO"; }
private:
    std::queue<int> pagesQueue;
};

#endif
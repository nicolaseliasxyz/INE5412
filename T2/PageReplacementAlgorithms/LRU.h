#ifndef LRU_H
#define LRU_H

#include "PageReplacementAlgorithm.h"
#include <list>

class LRU : public PageReplacementAlgorithm {
public:
    explicit LRU(RAM& ram) : PageReplacementAlgorithm(ram) {}

    void referencePage(int page) override;

    bool search(int key);

    std::string getName() const override { return "LRU"; }
private:
    std::list<int> pageList;
};

#endif
#ifndef PAGE_REPLACEMENT_ALGORITHM_H
#define PAGE_REPLACEMENT_ALGORITHM_H

#include "../RAM.h"

class PageReplacementAlgorithm {
public:
    virtual void referencePage(int page) = 0;
    virtual ~PageReplacementAlgorithm() = default;
    int getPageFaults() { return pageFaults; }
    virtual bool search(int key) = 0;
    virtual std::string getName() const = 0;
protected:
    explicit PageReplacementAlgorithm(RAM& ram) : ram_(ram) {}
    RAM& ram_;
    int pageFaults = 0;
};

#endif
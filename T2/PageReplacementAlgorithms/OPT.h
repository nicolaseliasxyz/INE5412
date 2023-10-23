#ifndef OPT_H
#define OPT_H

#include "PageReplacementAlgorithm.h"
#include <vector>
#include <unordered_set>

class OPT : public PageReplacementAlgorithm {
public:
    explicit OPT(RAM& ram, std::vector<int>& futureReferences) 
        : PageReplacementAlgorithm(ram), futureReferences_(futureReferences), currentIndex(0) {}

    void referencePage(int page) override;

    bool search(int key);
    
    std::string getName() const override { return "OPT"; }
private:
    int predict(int index);
    int currentIndex;
    std::vector<int> futureReferences_;
    std::unordered_set<int> pageSet;
};

#endif

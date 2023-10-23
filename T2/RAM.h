#ifndef RAM_H
#define RAM_H

#include <unordered_set>
#include <iostream>

class RAM {
public:
    explicit RAM(int numberOfFrames) : frameCapacity(numberOfFrames) {}
    ~RAM() = default;

    int getFrameCapacity() const {
        return frameCapacity;
    }

    void referenceFrame(int pageNumber) {
        frames.insert(pageNumber);
    }

    void eraseFrame(int oldestPage) {
        frames.erase(oldestPage);
    }

    bool isPageInRAM(int pageNumber) const {
        return frames.find(pageNumber) != frames.end();
    }

    void printFrames() const {
        for (int val : frames) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::unordered_set<int>::iterator findPageInRAM(int pageNumber) {
        return frames.find(pageNumber);
    }

    void reset() {
        frames.clear();
    }

private:
    // Jeito mais rapido de acessar algo que encontramos estrutura hash
    std::unordered_set<int> frames;
    int frameCapacity;
};

#endif

#ifndef TIME_H
#define TIME_H

class Time {
public:
    int globalTime = 0;

    void incrementGlobalTime() {
        globalTime++;
    }
};

#endif // TIME_H

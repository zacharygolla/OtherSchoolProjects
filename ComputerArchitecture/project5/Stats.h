#ifndef __STATS_H
#define __STATS_H
#include <iostream>
#include "Debug.h"
using namespace std;

enum PIPESTAGE { IF1 = 0, IF2 = 1, ID = 2, EXE1 = 3, EXE2 = 4, MEM1 = 5, 
                 MEM2 = 6, WB = 7, PIPESTAGES = 8 };

class Stats {
  private:
    long long cycles;
    int flushes;
    int bubbles;
    int stalls;

    int resultReg[PIPESTAGES];

  public:
    Stats();

    void clock();
    void flush(int count);
    void pipeStall(int n);
    void registerSrc(int r);
    void registerDest(int r);

    // getters
    long long getCycles() { return cycles; }
    int getFlushes() { return flushes; }
    int getBubbles() { return bubbles; }
    int getStalls() { return stalls; }

  private:
    void bubble();
};

#endif

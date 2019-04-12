#ifndef __STATS
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

    int memops;
    int branches;
    int taken;
    int RAW_HAZARDS;
    int EXEOne;
    int EXETwo;
    int MEMOne;
    int MEMTwo;
    int stalls;

    int resultReg[PIPESTAGES];
    int resultStage[PIPESTAGES];

  public:
    Stats();

    void clock();
    void flush(int count);
    void registerSrc(int r, PIPESTAGE stage);
    void registerDest(int r, PIPESTAGE stage);
    void pipeStall(int n);

    // counters
    void countMemOp() { memops++; }
    void countBranch() { branches++; }
    void countTaken() { taken++; }
    void countRawHazards() { RAW_HAZARDS++; }
    void countEXEOne() { EXEOne++; }
    void countEXETwo() { EXETwo++; }
    void countMEMOne() { MEMOne++; }
    void countMEMTwo() { MEMTwo++; }

    // getters
    long long getCycles() { return cycles; }
    int getFlushes() { return flushes; }
    int getBubbles() { return bubbles; }
    int getMemOps() { return memops; }
    int getBranches() { return branches; }
    int getTaken() { return taken; }
    int getRawHazards() { return RAW_HAZARDS; }
    int getEXEOne() { return EXEOne; }
    int getEXETwo() { return EXETwo; }
    int getMEMOne() { return MEMOne; }
    int getMEMTwo() { return MEMTwo; }
    int getStalls() { return stalls; }

  private:
    void bubble();
};

#endif

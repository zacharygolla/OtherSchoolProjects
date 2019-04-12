/******************************
 * Your Name Goes Here
 * CS 3339 - Semester Goes Here
 ******************************/
#include "Stats.h"

Stats::Stats() {
  cycles = PIPESTAGES - 1; // pipeline startup cost
  flushes = 0;
  bubbles = 0;

  memops = 0;
  branches = 0;
  taken = 0;

  for(int i = IF1; i < PIPESTAGES; i++) {
    resultReg[i] = -1;
  }
}

void Stats::clock(PIPESTAGE stage) {
  cycles++;

  // advance all pipeline flops
  for(int i = WB; i > stage; i--) {
    resultReg[i] = resultReg[i-1];
  }
  // inject no-op into IF1
  resultReg[stage] = -1;
}

void Stats::registerSrc(int r) {
  if(r==0)
      return;
    
    for(int i = EXE1; i < WB; i++) {
        if(r == resultReg[i]) {
            int ready = WB - i;
            
            while (ready > 0) {
                bubble();
                ready--;
            }
            break;
        }
    }
}

void Stats::registerDest(int r) {
  resultReg[ID] = r;
}

void Stats::flush(int count) { // count == how many ops to flush
  for (int i = 0; i < count; i ++) {
    flushes++;
    clock(IF1);
  }
}

void Stats::bubble() {
  bubbles++;
  clock(EXE1);
}

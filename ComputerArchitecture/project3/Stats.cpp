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

void Stats::clock() {
  cycles++;

  // advance all pipeline flops
  for(int i = WB; i > IF1; i--) {
    resultReg[i] = resultReg[i-1];
  }
  // inject no-op into IF1
  resultReg[IF1] = -1;
}

void Stats::registerSrc(int r) {
  int inserting_bubbles;
  for(int i = EXE1; i < WB; i++) {
    if (resultReg[i] == r && r != 0) {
      inserting_bubbles = WB - i;
      while (inserting_bubbles > 0) {
        bubble();
        inserting_bubbles--;
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
    clock();
  }
}

void Stats::bubble() {
  bubbles++;
  cycles++;
  for(int i = WB; i >= EXE2; i--) {
    resultReg[i] = resultReg[i-1];
  }
  resultReg[EXE1] = -1;
}

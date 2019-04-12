/******************************
* Name: Connor Oldmixon, cio12
* CS 3339 - Fall 2018
******************************/
#include "Stats.h"

Stats::Stats() {
  cycles = PIPESTAGES - 1; // pipeline startup cost
  flushes = 0;
  bubbles = 0;

  memops = 0;
  branches = 0;
  taken = 0;

  RAW_HAZARDS = 0;
  EXEOne = 0;
  EXETwo = 0;
  MEMOne = 0;
  MEMTwo = 0;

  for(int i = IF1; i < PIPESTAGES; i++) {
    resultReg[i] =  -1;
    resultStage[i] = -1;
  }
}

void Stats::clock() {
  cycles++;

  for(int i = WB; i > IF1; i--) {
    resultReg[i] = resultReg[i-1];
    resultStage[i] = resultStage[i-1];
  }
  resultReg[IF1] = -1;
  resultStage[IF1] = -1;
}

void Stats::registerSrc(int r, PIPESTAGE stage) {
int value = 0;
for(int i = EXE1; i < WB; i++){
    if(r == resultReg[i] && (r != 0)){
     
      if(i == EXE1){
        countEXEOne();
      }
      else if(i == EXE2){
        countEXETwo();
      }
      else if(i == MEM1){
        countMEMOne();
      }
      else if(i == MEM2){
        countMEMTwo();
      }

      value = (resultStage[i] - i) - (stage - ID);

      for(int j = 0; j < value; j++){
        bubble();
      }

      countRawHazards();
      
      break;
    }
  }
}

void Stats::registerDest(int r, PIPESTAGE stage) {
  resultReg[ID] = r;
  resultStage[ID] = stage;
}

void Stats::pipeStall(int n){
  stalls += n;
  cycles += n;
}

void Stats::flush(int count) { 
  for(int j = 0; j < count; j++){
    flushes++;
    clock();
  }
}

void Stats::bubble(){
  cycles++;
  bubbles++;

  for(int i = WB; i > EXE1; i--){
      resultReg[i] = resultReg[i-1];
      resultStage[i] = resultStage[i-1];
    }
  resultReg[EXE1] = -1;
  resultStage[EXE1] = -1;
}

/******************************
 * CacheStats.cpp submitted by: enter your first and last name and net ID
 * CS 3339 - Spring 2019
 * Project 4 Branch Predictor
 * Copyright 2019, all rights reserved
 * Updated by Lee B. Hinkle based on prior work by Martin Burtscher and Molly O'Neil
 ******************************/
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "CacheStats.h"
using namespace std;

CacheStats::CacheStats() {
  cout << "Cache Config: ";
  if(!CACHE_EN) {
    cout << "cache disabled" << endl;
  } else {
    cout << (SETS * WAYS * BLOCKSIZE) << " B (";
    cout << BLOCKSIZE << " bytes/block, " << SETS << " sets, " << WAYS << " ways)" << endl;
    cout << "  Latencies: Lookup = " << LOOKUP_LATENCY << " cycles, ";
    cout << "Read = " << READ_LATENCY << " cycles, ";
    cout << "Write = " << WRITE_LATENCY << " cycles" << endl;
  }

  loads = 0;
  stores = 0;
  load_misses = 0;
  store_misses = 0;
  writebacks = 0;

  index = 0;
  tag = 0;
  /* TODO: your code here */

  for(int i = 0; i < SETS; i++){
    for(int j = 0; j < WAYS; j++){
      theCache[i][j].valid = false;
      theCache[i][j].dirty = false;
      theCache[i][j].roundRobin = 0;
      theCache[i][j].tag = 0;
    }
  }
}

int CacheStats::access(uint32_t addr, ACCESS_TYPE type) {
  if(!CACHE_EN) { // cache is off
    return (type == LOAD) ? READ_LATENCY : WRITE_LATENCY;
  }

  index = (addr >> 5) & 0x7;
  tag = addr >> 8;
  int count = 0;
  int rr = 0;

  if(type == STORE){
    stores++;
  }
  else{
    loads++;
  }


  for(int i = 0; i < SETS; i++){
    if(i == index){
      for(int j = 0; j < WAYS; j++){
        //if a hit assign values accordingly
        if(theCache[i][j].tag == tag && theCache[i][j].valid){
          if(type == STORE)
            theCache[i][j].dirty = true;
          
          return 0;
        }
      }

      rr = theCache[i][0].roundRobin;
      //If the block has a dirty bit increment writebacks and increase count number
      if(theCache[i][rr].dirty == true){
        writebacks++;
        count += 10;
      }

      count += 30;
      theCache[i][rr].tag = tag;
      theCache[i][rr].valid = true;
      if(type == STORE){
        theCache[i][rr].dirty = true;
        store_misses++;
      }
      else{
	      theCache[i][rr].dirty = false;
        load_misses++;
      }
          
      if(theCache[i][0].roundRobin >= 3){
        theCache[i][0].roundRobin = 0;
      }
      else{
        theCache[i][0].roundRobin++;
      }

      return count;
    }
  }
}

void CacheStats::printFinalStats() {
  /* TODO: your code here (don't forget to drain the cache of writebacks) */
  for(int i = 0; i < SETS; i++){
    for(int j = 0; j < WAYS; j++){
      if(theCache[i][j].dirty){
        writebacks++;
      }
    }
  }  

  int accesses = loads + stores;
  int misses = load_misses + store_misses;
  cout << "Accesses: " << accesses << endl;
  cout << "  Loads: " << loads << endl;
  cout << "  Stores: " << stores << endl;
  cout << "Misses: " << misses << endl;
  cout << "  Load misses: " << load_misses << endl;
  cout << "  Store misses: " << store_misses << endl;
  cout << "Writebacks: " << writebacks << endl;
  cout << "Hit Ratio: " << fixed << setprecision(1) << 100.0 * (accesses - misses) / accesses;
  cout << "%" << endl;
}

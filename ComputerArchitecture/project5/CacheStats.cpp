/******************************
 * CacheStats.cpp submitted by: Zachary Golla ztg
 * CS 3339 - Spring 2019
 * Project 5 Data Cache Simulation
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

  //initialize index and tag to 0
  index = 0;
  tag = 0;

  //initiate the cache
  for(int i = 0; i < SETS; i++){
    for(int j = 0; j < WAYS; j++){
      cache[i][j].valid = false;
      cache[i][j].dirty = false;
      cache[i][j].roundRobin = 0;
      cache[i][j].tag = 0;
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

  if(type == STORE)
    stores++;
  else
    loads++;

  for(int i = 0; i < SETS; i++) {
    if(i == index) {
      for(int j = 0; j < WAYS; j++) {
        if(cache[i][j].tag == tag && cache[i][j].valid){
          if(type == STORE)
            cache[i][j].dirty = true;
          return 0;
        }
      }

      rr = cache[i][0].roundRobin;
      if(cache[i][rr].dirty == true) {
        writebacks++;
        count += 10;
      }

      count += 30;
      cache[i][rr].tag = tag;
      cache[i][rr].valid = true;
      if(type == STORE) {
        cache[i][rr].dirty = true;
        store_misses++;
      }
      else {
	      cache[i][rr].dirty = false;
        load_misses++;
      }
          
      if(cache[i][0].roundRobin >= 3){
        cache[i][0].roundRobin = 0;
      }
      else {
        cache[i][0].roundRobin++;
      }

      return count;
    }
  }
}

void CacheStats::printFinalStats() {
  /* TODO: your code here (don't forget to drain the cache of writebacks) */
  for(int i = 0; i < SETS; i++){
    for(int j = 0; j < WAYS; j++){
      if(cache[i][j].dirty){
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

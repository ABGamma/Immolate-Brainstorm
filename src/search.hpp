#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "instance.hpp"
#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include <atomic>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

const long long BLOCK_SIZE = 1000000;

class Search {
public:
  std::atomic<long long> seedsProcessed{0};
  std::atomic<long long> highScore{1};
  long long printDelay = 10000000;
  std::function<int(Instance)> filter;
  std::atomic<bool> found{
      false};     // Atomic flag to signal when a solution is found
  Seed foundSeed; // Store the found seed
  bool exitOnFind = false;
  Seed startSeed;
  int numThreads;
  long long numSeeds;
  std::mutex mtx;
  std::atomic<long long> nextBlock{
      0}; // Shared index for the next block to be processed

  Search(std::function<int(Instance)> f, std::string seed = 0, int t = 1,
         long long n = 2318107019761) {
    filter = f;
    numThreads = t;
    numSeeds = n;
    startSeed = Seed(seed);
  }

  void searchBlock(long long start, long long end) {
    Seed s(start);
    Instance inst(s);
    for (long long i = start; i < end; ++i) {
      if (found)
        return; // Exit if a solution is found
      // Perform the search on the seed
      int result = filter(inst);
      std::lock_guard<std::mutex> lock(mtx);
      if (result >= highScore) {
        highScore = result;
        foundSeed = s;
        std::cout << "Found seed: " << s.tostring() << " (" << result << ")"
                  << std::endl;
        if (exitOnFind) {
          found = true;
          return;
        }
      }
      seedsProcessed++;
      if (seedsProcessed % printDelay == 0) {
        std::cout << "Seeds processed: " << seedsProcessed << std::endl;
      }
      s.next();      // Instead of destroying seeds, update directly
      inst.reset(s); // Reset the instance to the next seed
    }
  }

  std::string search() {
    std::vector<std::thread> threads;
    long long totalBlocks = (numSeeds + BLOCK_SIZE - 1) / BLOCK_SIZE;
    long long blocksPerThread = (totalBlocks + numThreads - 1) / numThreads;

    for (int t = 0; t < numThreads; ++t) {
      long long startBlock = t * blocksPerThread;
      long long endBlock = std::min(startBlock + blocksPerThread, totalBlocks);
      threads.emplace_back([this, startBlock, endBlock]() {
        for (long long block = startBlock; block < endBlock; ++block) {
          long long start = block * BLOCK_SIZE;
          long long end = std::min(start + BLOCK_SIZE, numSeeds);
          searchBlock(start, end);
        }
      });
    }
    for (auto &thread : threads) {
      thread.join();
    }
    return foundSeed.tostring();
  }
};

#endif
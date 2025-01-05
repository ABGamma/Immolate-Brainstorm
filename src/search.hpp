#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "instance.hpp"
#include <atomic>
#include <functional>
#include <iostream>
#include <thread>

struct Search {
  long long numSeeds;
  int numThreads;
  Seed startSeed; // The first entry indicates the length of the seed
  long long seedsProcessed = 0;
  long long highScore = 0;
  long long printDelay = 10000000;
  std::function<int(Instance)> filter;
  std::atomic<bool> found{
      false};     // Atomic flag to signal when a solution is found
  Seed foundSeed; // Store the found seed
  bool exitOnFind = false;

  void searching_thread(int ID) {
    Seed seed = Seed(startSeed.getID() + ID);
    Instance inst(seed);
    for (int i = 0; i < (numSeeds - ID) / numThreads; i++) {
      if (exitOnFind && found.load())
        return; // Exit if another thread found a valid seed
      inst.reset(seed);
      long score = filter(inst);
      if (score >= highScore && score > 0 && (!exitOnFind || !found.load())) {
        found.store(true);
        foundSeed = seed;
        highScore = score;
        std::cout << "Found seed: " << seed.tostring() << " (" << score << ")"
                  << std::endl;
        if (exitOnFind) {
          std::cout << "Seed found, exiting..." << std::endl;
          return; // Exit thread after finding the solution
        }
      }
      seedsProcessed++;
      if (seedsProcessed % printDelay == 0) {
        std::cout << seedsProcessed << " seeds searched" << std::endl;
      };
      //seed.next(numThreads);
      seed.next(); // For now, run single-threaded
    }
  }

  std::string search() {
    std::vector<std::thread> threads;
    // For now, run single-threaded
    //for (int i = 0; i < numThreads; ++i) {
      // Bind the member function with the instance of the class
      auto bound_thread_func = std::bind(&Search::searching_thread, this, 0);//i);
      // Use a lambda function to capture the bound function and start a thread
      threads.emplace_back([bound_thread_func]() { bound_thread_func(); });
    //}
    for (std::thread &t : threads) {
      t.join();
    }
    return foundSeed.tostring(); // Return the found seed
  }

  Search(std::function<int(Instance)> f) {
    filter = f;
    startSeed = Seed(0);
    numThreads = 1;
    numSeeds = 2318107019761;
  };
  Search(std::function<int(Instance)> f, int t) {
    filter = f;
    startSeed = Seed(0);
    numThreads = t;
    numSeeds = 2318107019761;
  };
  Search(std::function<int(Instance)> f, int t, long long n) {
    filter = f;
    startSeed = Seed(0);
    numThreads = t;
    numSeeds = n;
  };
  Search(std::function<int(Instance)> f, std::string seed, int t, long long n) {
    filter = f;
    startSeed = Seed(0);
    numThreads = t;
    numSeeds = n;
  };
};

#endif
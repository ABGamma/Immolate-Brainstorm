#include "seed.hpp"
#include <iostream>

Seed::Seed() { seed.fill(-1); }

Seed::Seed(std::string strSeed) {
  seed.fill(-1);
  // Note: Assumes this is safe
  for (int i = 0; i < strSeed.size(); i++) {
    seed[strSeed.size() - 1 - i] = charSeeds[strSeed[i]];
  }
}

Seed::Seed(long long id) {
  for (int i = 0; i < 8; i++) {
    if (id > 0) {
      seed[i] = (id - 1) / idCoeff[i];
      id -= 1 + seed[i] * idCoeff[i];
    } else {
      seed[i] = -1;
    }
  }
}

std::string Seed::tostring() {
  std::string strSeed;
  for (int i = 7; i >= 0; i--) {
    if (seed[i] != -1) {
      strSeed.push_back(seedChars[seed[i]]);
    }
  }
  return strSeed;
}

void Seed::debugprint() {
  for (int i = 0; i < 8; i++) {
    std::cout << seed[i] << " ";
  }
  std::cout << std::endl;
}

long long Seed::getID() {
  long long id = 0;
  for (int i = 0; i <= 7; i++) {
    if (seed[i] >= 0) {
      id += idCoeff[i] * seed[i] + 1;
    }
  }
  return id;
}

void Seed::next() {
  long long newID = (getID() + 1) % 2318107019761;
  *this = Seed(newID);
}

void Seed::next(int x) {
  long long newID = (getID() + x) % 2318107019761;
  *this = Seed(newID);
}
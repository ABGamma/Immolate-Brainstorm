#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include <cstdint>
#include <string>

const uint64_t MAX_UINT64 = 18446744073709551615ull;

typedef union DoubleLong {
  double dbl;
  uint64_t ulong;
} dbllong;

struct LuaRandom {
  uint64_t state[4];
  LuaRandom(double seed);
  LuaRandom();
  uint64_t _randint();
  uint64_t randdblmem();
  double random();
  int randint(int min, int max);
};

#define DBL_EXPO 0x7FF0000000000000
#define DBL_MANT 0x000FFFFFFFFFFFFF

#define DBL_EXPO_SZ 11
#define DBL_MANT_SZ 52

#define DBL_EXPO_BIAS 1023

#if defined(_MSC_VER)
#include <intrin.h>
#pragma intrinsic(_BitScanReverse64)
#endif

int portable_clzll(uint64_t x);
double fract(double x);
double pseudohash(std::string s);
double pseudohash_from(std::string s, double num);
double pseudostep(char s, int pos, double num);
std::string anteToString(int a);
double round13(double x);

#if (defined(__GNUC__) || defined(__clang__)) && defined(__AVX512F__)
#include <array>
typedef union DoubleLongSIMD {
  std::array<double,8> dbl;
  std::array<uint64_t,8> ulong;
} dbllongSIMD;
struct LuaRandomSIMD {
  std::array<std::uint64_t,32> state;
  LuaRandomSIMD(std::array<double,8> seed);
  LuaRandomSIMD();
  std::array<uint64_t,8> _randint();
  std::array<uint64_t,8> randdblmem();
  std::array<double,8> random();
  std::array<int,8> randint(int min, int max);
};
#endif

#endif // UTIL_HPP
#include <array>
#include <string>
#include <algorithm>
// Seed helper class
// Caches hashing info recursively to save speed
// Because of that, also has an intersting order for seeds:
// <empty>, 1, 11, 111, ..., 11111111, 21111111, 31111111, ..., Z1111111, 2111111, 12111111, ..., ZZ111111, 211111, ..., ZZZZZZZZ
const std::string seedChars = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::array<int, 128> charSeeds = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1,
    -1,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};
const std::array<long long, 8> idCoeff = {
    66231629136, 1892332261, 54066636, 1544761, 44136, 1261, 36, 1
};

struct Seed {
    // -1 is blank, 0 to 34 represent valid characters
    // To aid in hashing, stored right to left
    std::array<int, 8> seed;

    Seed() {
        seed.fill(-1);
    }

    Seed(std::string strSeed) {
        seed.fill(-1);
        // Note: Assumes this is safe
        for (int i = 0; i < strSeed.size(); i++) {
            seed[strSeed.size()-1-i] = charSeeds[strSeed[i]];
        }
    }

    Seed(long long id) {
        for (int i = 0; i < 8; i++) {
            if (id > 0) {
                seed[i] = (id-1) / idCoeff[i];
                id -= 1 + seed[i] * idCoeff[i];
            } else {
                seed[i] = -1;
            }
        }
    }

    std::string tostring() {
        std::string strSeed;
        for (int i = 7; i >= 0; i--) {
            if (seed[i] != -1) {
                strSeed.push_back(seedChars[seed[i]]);
            }
        }
        return strSeed;
    }
    void debugprint() {
        for (int i = 0; i < 8; i++) {
            std::cout << seed[i] << " ";
        }
        std::cout << std::endl;
    }
    
    long long getID() {
        long long id = 0;
        for (int i = 0; i <= 7; i++) {
            if (seed[i] >= 0) {
                id += idCoeff[i] * seed[i] + 1;
            }
        }
        return id;
    }


    // This is a temporary function; it will be optimized and work with caches later
    void next() {
        long long newID = (getID() + 1) % 2318107019761;
        *this = Seed(newID);
    }

    void next(int x) {
        long long newID = (getID() + x) % 2318107019761;
        *this = Seed(newID);
    }
};
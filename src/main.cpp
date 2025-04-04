// For right now, this is just me tinkering with SIMD, trying to understand how it works.
#include "vector.hpp"
#include <iostream>
#include <chrono>

template <typename VectorType, typename NumberType>
void perform_operations() {
    VectorType a({1,2,3,4,5,6,7,8});
    VectorType b({9,8,7,6,5,4,3,2});
    VectorType _add = a + b;
    VectorType _sub = a - b;
    VectorType _mul = a * b;
    VectorType _div = a / b;
    VectorType _and = a & b;
    VectorType _or = a | b;
    VectorType _xor = a ^ b;
    VectorType _eq = a == b;
    VectorType _neq = a != b;
    VectorType _lt = a < b;
    VectorType _gt = a > b;
    VectorType _lte = a <= b;
    VectorType _gte = a >= b;
    std::cout << "a = " << a.to_string() << std::endl;
    std::cout << "b = " << b.to_string() << std::endl;
    std::cout << "a + b = " << _add.to_string() << std::endl;
    std::cout << "a - b = " << _sub.to_string() << std::endl;
    std::cout << "a * b = " << _mul.to_string() << std::endl;
    std::cout << "a / b = " << _div.to_string() << std::endl;
    std::cout << "a & b = " << _and.to_string() << std::endl;
    std::cout << "a | b = " << _or.to_string() << std::endl;
    std::cout << "a ^ b = " << _xor.to_string() << std::endl;
    std::cout << "a == b = " << _eq.to_string() << std::endl;
    std::cout << "a != b = " << _neq.to_string() << std::endl;
    std::cout << "a < b = " << _lt.to_string() << std::endl;
    std::cout << "a > b = " << _gt.to_string() << std::endl;
    std::cout << "a <= b = " << _lte.to_string() << std::endl;
    std::cout << "a >= b = " << _gte.to_string() << std::endl;

    // Speed test, add 1M vectors and time it
    VectorType c({1,2,3,4,5,6,7,8});
    VectorType d({8,7,6,5,4,3,2,1});
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000000; ++i) {
        c += d;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "8B additions took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms (SIMD)" << std::endl;
    std::cout << "c = " << c.to_string() << std::endl;

    //Compare with just numbers
    NumberType e[] = {1,2,3,4,5,6,7,8};
    NumberType f[] = {8,7,6,5,4,3,2,1};
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000000; ++i) {
        for (int j = 0; j < 8; ++j) {
            e[j] += f[j];
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "8B additions took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms (basic)" << std::endl;
    std::cout << "e = [";
    for (int i = 0; i < 8; ++i) {
        std::cout << e[i];
        if (i < 7) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "\nTesting with VectorInt (integer):" << std::endl;
    // I used AI and some of its AVX2 choices don't actually work with AVX2...
    // This will need more fixing later
    perform_operations<VectorInt,uint64_t>();

    std::cout << "Testing with Vector (floating-point):" << std::endl;
    perform_operations<Vector,double>();

    return 0;
}

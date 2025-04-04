// Defines a wrapper class for working with SIMD vectors for AVX2 and AVX-512.
#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <immintrin.h>
#include <initializer_list>
#include <iostream>
#include <string>

typedef union DoubleLong {
    double dbl;
    uint64_t ulong;
} dbllong;

//Vector of doubles
struct Vector {
    alignas(64) double data[8];
    __m512d avx512_data;
    __m256d avx2_data[2];
    Vector() {
        Vector({0,0,0,0,0,0,0,0});
    }
    Vector(double data) {
        for (int i = 0; i < 8; ++i) {
            this->data[i] = data;
        }
        #ifdef __AVX512F__
            avx512_data = _mm512_set1_pd(data);
        #elif __AVX2__
            avx2_data[0] = _mm256_set1_pd(data);
            avx2_data[1] = _mm256_set1_pd(data);
        #endif
    }
    Vector(const double* data) {
        std::copy(data, data + 8, this->data);
        #ifdef __AVX512F__
            avx512_data = _mm512_load_pd(data);
        #elif __AVX2__
            avx2_data[0] = _mm256_load_pd(data);
            avx2_data[1] = _mm256_load_pd(data + 4);
        #endif
    };
    Vector(std::initializer_list<double> init) {
        std::copy(init.begin(), init.end(), this->data);
        #ifdef __AVX512F__
            avx512_data = _mm512_load_pd(data);
        #elif __AVX2__
            avx2_data[0] = _mm256_load_pd(data);
            avx2_data[1] = _mm256_load_pd(data + 4);
        #endif
    };

    std::string to_string() {
        // Parse back to data array
        #ifdef __AVX512F__
            _mm512_storeu_pd(data, avx512_data);
        #elif __AVX2__
            _mm256_storeu_pd(data, avx2_data[0]);
            _mm256_storeu_pd(data + 4, avx2_data[1]);
        #endif
        std::string result = "[";
        for (int i = 0; i < 8; ++i) {
            result += std::to_string(data[i]);
            if (i < 7) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    };

    //Regular old operators
    Vector operator+(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_add_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_add_pd(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_add_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] + other.data[i];
            }
        #endif
        return result;
    }

    Vector operator-(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_sub_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_sub_pd(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_sub_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] - other.data[i];
            }
        #endif
        return result;
    }
    
    Vector operator*(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_mul_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_mul_pd(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_mul_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] * other.data[i];
            }
        #endif
        return result;
    }

    Vector operator/(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_div_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_div_pd(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_div_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] / other.data[i];
            }
        #endif
        return result;
    }

    Vector operator=(const Vector& other) {
        #ifdef __AVX512F__
            avx512_data = other.avx512_data;
        #elif __AVX2__
            avx2_data[0] = other.avx2_data[0];
            avx2_data[1] = other.avx2_data[1];
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] = other.data[i];
            }
        #endif
        return *this;
    }

    Vector operator+=(const Vector& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_add_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_add_pd(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_add_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] += other.data[i];
            }
        #endif
        return *this;
    }

    Vector operator-=(const Vector& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_sub_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_sub_pd(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_sub_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] -= other.data[i];
            }
        #endif
        return *this;
    }

    Vector operator*=(const Vector& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_mul_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_mul_pd(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_mul_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] *= other.data[i];
            }
        #endif
        return *this;
    }

    Vector operator/=(const Vector& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_div_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_div_pd(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_div_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] /= other.data[i];
            }
        #endif
        return *this;
    }

    //Bitwise operators
    Vector operator&(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_and_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_and_pd(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_and_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = (uint64_t)data[i] & (uint64_t)other.data[i];
            }
        #endif
        return result;
    }

    Vector operator|(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_or_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_or_pd(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_or_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = (uint64_t)data[i] | (uint64_t)other.data[i];
            }
        #endif
        return result;
    }

    Vector operator^(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_xor_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_xor_pd(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_xor_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = (uint64_t)data[i] ^ (uint64_t)other.data[i];
            }
        #endif
        return result;
    }

    Vector operator&=(const Vector& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_and_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_and_pd(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_and_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] = (uint64_t)data[i] & (uint64_t)other.data[i];
            }
        #endif
        return *this;
    }

    Vector operator|=(const Vector& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_or_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_or_pd(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_or_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] = (uint64_t)data[i] | (uint64_t)other.data[i];
            }
        #endif
        return *this;
    }

    Vector operator^=(const Vector& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_xor_pd(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_xor_pd(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_xor_pd(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] = (uint64_t)data[i] ^ (uint64_t)other.data[i];
            }
        #endif
        return *this;
    }

    //Comparison operators
    Vector operator==(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmp_pd_mask(avx512_data, other.avx512_data, _CMP_EQ_OQ);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_cmp_pd(avx2_data[0], other.avx2_data[0], _CMP_EQ_OQ);
            result.avx2_data[1] = _mm256_cmp_pd(avx2_data[1], other.avx2_data[1], _CMP_EQ_OQ);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] == other.data[i];
            }
        #endif
        return result;
    }

    Vector operator!=(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmp_pd_mask(avx512_data, other.avx512_data, _CMP_NEQ_OQ);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_cmp_pd(avx2_data[0], other.avx2_data[0], _CMP_NEQ_OQ);
            result.avx2_data[1] = _mm256_cmp_pd(avx2_data[1], other.avx2_data[1], _CMP_NEQ_OQ);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] != other.data[i];
            }
        #endif
        return result;
    }

    Vector operator<(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmp_pd_mask(avx512_data, other.avx512_data, _CMP_LT_OQ);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_cmp_pd(avx2_data[0], other.avx2_data[0], _CMP_LT_OQ);
            result.avx2_data[1] = _mm256_cmp_pd(avx2_data[1], other.avx2_data[1], _CMP_LT_OQ);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] < other.data[i];
            }
        #endif
        return result;
    }

    Vector operator<=(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmp_pd_mask(avx512_data, other.avx512_data, _CMP_LE_OQ);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_cmp_pd(avx2_data[0], other.avx2_data[0], _CMP_LE_OQ);
            result.avx2_data[1] = _mm256_cmp_pd(avx2_data[1], other.avx2_data[1], _CMP_LE_OQ);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] <= other.data[i];
            }
        #endif
        return result;
    }

    Vector operator>(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmp_pd_mask(avx512_data, other.avx512_data, _CMP_GT_OQ);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_cmp_pd(avx2_data[0], other.avx2_data[0], _CMP_GT_OQ);
            result.avx2_data[1] = _mm256_cmp_pd(avx2_data[1], other.avx2_data[1], _CMP_GT_OQ);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] > other.data[i];
            }
        #endif
        return result;
    }

    Vector operator>=(const Vector& other) const {
        Vector result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmp_pd_mask(avx512_data, other.avx512_data, _CMP_GE_OQ);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_cmp_pd(avx2_data[0], other.avx2_data[0], _CMP_GE_OQ);
            result.avx2_data[1] = _mm256_cmp_pd(avx2_data[1], other.avx2_data[1], _CMP_GE_OQ);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] >= other.data[i];
            }
        #endif
        return result;
    }
};
// Vector of uint64
struct VectorInt {
    alignas(64) uint64_t data[8];
    __m512i avx512_data;
    __m256i avx2_data[2];
    VectorInt() {
        VectorInt({0,0,0,0,0,0,0,0});
    }
    VectorInt(uint64_t data) {
        for (int i = 0; i < 8; ++i) {
            this->data[i] = data;
        }
        #ifdef __AVX512F__
            avx512_data = _mm512_set1_epi64(data);
        #elif __AVX2__
            avx2_data[0] = _mm256_set1_epi64x(data);
            avx2_data[1] = _mm256_set1_epi64x(data);
        #endif
    }
    VectorInt(const uint64_t* data) {
        std::copy(data, data + 8, this->data);
        #ifdef __AVX512F__
            avx512_data = _mm512_loadu_si512(data);
        #elif __AVX2__
            avx2_data[0] = _mm256_loadu_si256((__m256i*)data);
            avx2_data[1] = _mm256_loadu_si256((__m256i*)(data + 4));
        #endif
    };
    VectorInt(std::initializer_list<uint64_t> init) {
        std::copy(init.begin(), init.end(), data);
        #ifdef __AVX512F__
            avx512_data = _mm512_loadu_si512(data);
        #elif __AVX2__
            avx2_data[0] = _mm256_loadu_si256((__m256i*)data);
            avx2_data[1] = _mm256_loadu_si256((__m256i*)(data + 4));
        #endif
    };
    std::string to_string() {
        // Parse back to data array
        #ifdef __AVX512F__
            _mm512_storeu_si512(data, avx512_data);
        #elif __AVX2__
            _mm256_storeu_si256((__m256i*)data, avx2_data[0]);
            _mm256_storeu_si256((__m256i*)(data + 4), avx2_data[1]);
        #endif
        std::string result = "[";
        for (int i = 0; i < 8; ++i) {
            result += std::to_string(data[i]);
            if (i < 7) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    };
    //Regular old operators
    VectorInt operator+(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_add_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_add_epi64(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_add_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] + other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator-(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_sub_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_sub_epi64(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_sub_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] - other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator*(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_mullo_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            __m256i low0 = _mm256_mul_epu32(avx2_data[0], other.avx2_data[0]);
            __m256i high0 = _mm256_mul_epu32(_mm256_srli_epi64(avx2_data[0], 32), _mm256_srli_epi64(other.avx2_data[0], 32));
            result.avx2_data[0] = _mm256_blend_epi32(low0, _mm256_slli_epi64(high0, 32), 0xAA);

            __m256i low1 = _mm256_mul_epu32(avx2_data[1], other.avx2_data[1]);
            __m256i high1 = _mm256_mul_epu32(_mm256_srli_epi64(avx2_data[1], 32), _mm256_srli_epi64(other.avx2_data[1], 32));
            result.avx2_data[1] = _mm256_blend_epi32(low1, _mm256_slli_epi64(high1, 32), 0xAA);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] * other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator/(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_div_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_div_epi64(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_div_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] / other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator=(const VectorInt& other) {
        #ifdef __AVX512F__
            avx512_data = other.avx512_data;
        #elif __AVX2__
            avx2_data[0] = other.avx2_data[0];
            avx2_data[1] = other.avx2_data[1];
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] = other.data[i];
            }
        #endif
        return *this;
    }
    VectorInt operator+=(const VectorInt& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_add_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_add_epi64(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_add_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] += other.data[i];
            }
        #endif
        return *this;
    }
    VectorInt operator-=(const VectorInt& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_sub_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_sub_epi64(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_sub_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] -= other.data[i];
            }
        #endif
        return *this;
    }
    VectorInt operator*=(const VectorInt& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_mullo_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_mullo_epi64(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_mullo_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] *= other.data[i];
            }
        #endif
        return *this;
    }
    VectorInt operator/=(const VectorInt& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_div_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_div_epi64(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_div_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] /= other.data[i];
            }
        #endif
        return *this;
    }
    //Bitwise operators
    VectorInt operator&(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_and_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_and_epi64(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_and_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] & other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator|(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_or_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_or_epi64(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_or_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] | other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator^(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_xor_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.avx2_data[0] = _mm256_xor_epi64(avx2_data[0], other.avx2_data[0]);
            result.avx2_data[1] = _mm256_xor_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] ^ other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator&=(const VectorInt& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_and_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_and_epi64(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_and_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] &= other.data[i];
            }
        #endif
        return *this;
    }
    VectorInt operator|=(const VectorInt& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_or_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_or_epi64(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_or_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] |= other.data[i];
            }
        #endif
        return *this;
    }
    VectorInt operator^=(const VectorInt& other) {
        #ifdef __AVX512F__
            avx512_data = _mm512_xor_epi64(avx512_data, other.avx512_data);
        #elif __AVX2__
            avx2_data[0] = _mm256_xor_epi64(avx2_data[0], other.avx2_data[0]);
            avx2_data[1] = _mm256_xor_epi64(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                data[i] ^= other.data[i];
            }
        #endif
        return *this;
    }
    //Comparison operators
    VectorInt operator==(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmpeq_epi64_mask(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.data[0] = _mm256_cmpeq_epi64_mask(avx2_data[0], other.avx2_data[0]);
            result.data[1] = _mm256_cmpeq_epi64_mask(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] == other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator!=(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmpneq_epi64_mask(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.data[0] = _mm256_cmpneq_epi64_mask(avx2_data[0], other.avx2_data[0]);
            result.data[1] = _mm256_cmpneq_epi64_mask(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] != other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator<(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmplt_epi64_mask(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.data[0] = _mm256_cmplt_epi64_mask(avx2_data[0], other.avx2_data[0]);
            result.data[1] = _mm256_cmplt_epi64_mask(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] < other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator<=(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmple_epi64_mask(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.data[0] = _mm256_cmple_epi64_mask(avx2_data[0], other.avx2_data[0]);
            result.data[1] = _mm256_cmple_epi64_mask(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] <= other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator>(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmpgt_epi64_mask(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.data[0] = _mm256_cmpgt_epi64_mask(avx2_data[0], other.avx2_data[0]);
            result.data[1] = _mm256_cmpgt_epi64_mask(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] > other.data[i];
            }
        #endif
        return result;
    }
    VectorInt operator>=(const VectorInt& other) const {
        VectorInt result;
        #ifdef __AVX512F__
            result.avx512_data = _mm512_cmpge_epi64_mask(avx512_data, other.avx512_data);
        #elif __AVX2__
            result.data[0] = _mm256_cmpge_epi64_mask(avx2_data[0], other.avx2_data[0]);
            result.data[1] = _mm256_cmpge_epi64_mask(avx2_data[1], other.avx2_data[1]);
        #else
            for (int i = 0; i < 8; ++i) {
                result.data[i] = data[i] >= other.data[i];
            }
        #endif
        return result;
    }
};

#endif // VECTOR_HPP
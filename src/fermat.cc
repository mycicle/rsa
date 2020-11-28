#ifndef FERMAT
#define FERMAT

#include <cmath>
#include <random>
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/random.hpp"
#include "powermod.cc"

typedef boost::multiprecision::int1024_t int1024_t;
boost::random::independent_bits_engine<boost::random::mt19937, 1024, int1024_t> gen;

bool fermat(int1024_t p, int k) {
    for (int i = 0; i < k; i++) {
        int1024_t a = gen();
        if (powermod(a, p-1, p) != 1) {
            return false;
        }
    }
    return true;
}

#endif
#ifndef POWERMOD
#define POWERMOD

#include <cmath>
#include "boost/multiprecision/cpp_int.hpp"

typedef boost::multiprecision::int1024_t int1024_t;

int1024_t powermod(int1024_t x, int1024_t n, int1024_t m) {
    namespace mp = boost::multiprecision;
    // for x^n mod m
    int1024_t prod = 1;
    while (n > 0) {
        if (n % 2 != 0) {
            prod = (prod*x) % m;
        }
        x = (int1024_t) mp::pow(mp::cpp_int(x), 2) % m;
        n = n/2;
    }
    return prod;
}

#endif
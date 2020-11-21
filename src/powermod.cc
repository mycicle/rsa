#ifndef POWERMOD
#define POWERMOD

#include <cmath>
#include "boost/multiprecision/cpp_int.hpp"

typedef boost::multiprecision::uint1024_t uint1024_t;

uint1024_t powermod(uint1024_t x, uint1024_t n, uint1024_t m) {
    namespace mp = boost::multiprecision;
    // for x^n mod m
    uint1024_t prod = 1;
    while (n > 0) {
        if (n % 2 != 0) {
            prod = (prod*x) % m;
        }
        x = (uint1024_t) mp::pow(mp::cpp_int(x), 2) % m;
        n = n/2;
    }
    return prod;
}

int main() {
    return 0;
}
#endif
#pragma once
#include <cmath>
#include "boost/multiprecision/cpp_int.hpp"

typedef boost::multiprecision::uint256_t uint256_t;

uint256_t powermod(uint256_t x, uint256_t n, uint256_t m) {
    namespace mp = boost::multiprecision;
    // for x^n mod m
    uint256_t prod = 1;
    while (n > 0) {
        if (n % 2 != 0) {
            prod = (prod*x) % m;
        }
        x = (uint256_t) mp::pow(mp::cpp_int(x), 2) % m;
        n = n/2;
    }
    return prod;
}
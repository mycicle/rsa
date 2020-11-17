#pragma once
#include <cmath>
#include <random>
#include "boost/multiprecision/cpp_int.hpp"
#include "powermod.cc"

std::default_random_engine gen(0);

typedef boost::multiprecision::uint256_t uint256_t;

bool fermat(uint256_t p, int k) {
    std::uniform_int_distribution<uint256_t> distribution((uint256_t)(p-1)/(10), p-1);
    for (int i = 0; i < k; i++) {
        uint256_t a = distribution(gen);
        if (powermod(a, p-1, p) != 1) {
            return false;
        }
    }
    return true;
}
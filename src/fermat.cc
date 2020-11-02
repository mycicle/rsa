#pragma once
#include <cmath>
#include <random>
#include "powermod.cc"

std::default_random_engine gen(0);

bool fermat(long long int p, int k) {
    std::uniform_int_distribution<long long int> distribution((long long int)(p-1)/(10), p-1);
    for (int i = 0; i < k; i++) {
        long long int a = distribution(gen);
        if (powermod(a, p-1, p) != 1) {
            return false;
        }
    }
    return true;
}
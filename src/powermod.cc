#pragma once
#include <cmath>

long double powermod(long long int x, long long int n, long long int m) {
    // for x^n mod m
    long long int prod = 1;
    while (n > 0) {
        if (n % 2 != 0) {
            prod = (prod*x) % m;
        }
        x = (long long int)std::pow(x, 2) % m;
        n = n/2;
    }
    return prod;
}
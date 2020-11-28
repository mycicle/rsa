#include <iostream>
#include <string>
#include "fermat.cc"
#include "powermod.cc"
#include "generate_message.cc"
#include "modinv.cc"
#include "genprime.cc"
#include "boost/multiprecision/cpp_int.hpp"
#include <cmath>

typedef boost::multiprecision::int1024_t int1024_t;
typedef boost::multiprecision::int512_t int512_t;

struct rsaKeyPair {
    int1024_t modulus; // N = p*q
    int512_t publicExponent; // e, number less than and coprime to (p-1)(q-1)
    int1024_t privateExponent; // d, such that e*d = 1 mod ((p-1)*(q-1)) 
};

rsaKeyPair generateKeyPair() {

    int bits = 1024;
    // up first lets choose e
    int512_t e = 65537; // https://www.di-mgt.com.au/rsa_alg.html
    // now we have to choose two random prime numbers
    int512_t p;
    int512_t q;
    do {
        p = genprime(std::floor(bits/2)); //genprime generates a prime of (input) bits in length
    } while ((p % e) == 1);
    do {
        q = genprime(std::floor(bits - bits/2));
    } while ((q % e) == 1);

    int1024_t n = p*q;
    int1024_t l = (p-1)*(q-1);

    int1024_t d = modInverse(e, l);

    return {
        n,
        e,
        d
    };
}

int main() {
    rsaKeyPair keyPair = generateKeyPair();
    return 0;
}
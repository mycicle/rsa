#include <gmp.h>

#define MODULUS_SIZE 2048
#define BUFFER_SIZE ((MODULUS_SIZE/8) / 2) // number of bytes in n and p

typedef struct {
    mpz_t e; // public exponent
    mpz_t n; // modulus
} public_key;

typedef struct {
    mpz_t p; // p
    mpz_t q; // q
    mpz_t L; // (p-1)*(q-1)
    mpz_t n; // p*q
    mpz_t d; // d*e mod L = 1
    mpz_t e; // public exponent
} private_key;


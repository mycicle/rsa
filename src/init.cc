// gcc program_name.c -lgmp
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <chrono>

#include "utils.cc"

void gen_random_prime(mpz_t &e, mpz_t &output) {
    mpz_t t1;
    mpz_init(t1);
    mpz_t t2;
    mpz_init(t2);

    // use mpz_nextprime https://crypto.stackexchange.com/questions/2532/generating-random-primes
    // so lets get a big number, we need to do byte stuff for this!
    char num_buffer[BUFFER_SIZE];

    for (int i = 0; i < BUFFER_SIZE; i++) {
        num_buffer[i] = rand() % 0xFF;
    }

    // so lets make this number really big by setting the top bit
    // and the third bit to 1, make sure that its odd by setting the last 
    // bit to one
    num_buffer[0] |= 0xA0;    // 1010 0000
    num_buffer[BUFFER_SIZE-1] |= 0x01; // 0000 0001

    mpz_import(t1, BUFFER_SIZE, 1, sizeof(char), 0, 0, num_buffer);
    mpz_nextprime(output, t1);

    // Time to validate p!
    // so p mod e == 1 must be true
    mpz_mod(t2, output, e);
    while(!mpz_cmp_ui(t2, 1)) {
        mpz_nextprime(output, output);
        mpz_mod(t2, output, e);
    }
}

private_key gen_private_key() {
    private_key priv;
    mpz_init(priv.p); mpz_init(priv.q);
    mpz_init(priv.d); mpz_init(priv.e);
    mpz_init(priv.L); mpz_init(priv.n);
    // set private key according to common convention, works for me! 
    mpz_set_ui(priv.e, 257); // https://www.di-mgt.com.au/rsa_alg.html

    mpz_t p; 
    mpz_t q;
    mpz_init(p);
    mpz_init(q);

    gen_random_prime(priv.e, p);

    do {
        gen_random_prime(priv.e, q);
    } while(mpz_cmp(p, q) == 0); // if p and q are the same, then generate q again

    mpz_set(priv.p, p);
    mpz_set(priv.q, q);

    // so now we have e, p, and q
    // lets get L = (p-1)*(q-1)
    mpz_t pm; 
    mpz_t qm;
    mpz_init(pm);
    mpz_init(qm);

    mpz_sub_ui(pm, priv.p, 1);
    mpz_sub_ui(qm, priv.q, 1);
    mpz_mul(priv.L, pm, qm);

    // lets get n = p * q
    mpz_mul(priv.n, priv.p, priv.q);

    // and now we need d, where d^e mod L = 1
    // since d * C = 1, d is the mutliplicative inverse of C
    // where C = e mod L
    // Calculate d (multiplicative inverse of e mod L)
    // https://stackoverflow.com/questions/38155864/understanding-mpz-invert
    if(mpz_invert(priv.d, priv.e, priv.L) == 0)
    {
        mpz_gcd(pm, priv.e, priv.L);
        std::cout << "gcd(e, L): " << mpz_get_str(NULL, 16, pm) << '\n';
        std::cout << "Could not find the modular multiplicative inverse\n";
    }

    return priv;
}   

public_key gen_public_key(const private_key &priv) {
    public_key pub;
    mpz_init(pub.e); mpz_init(pub.n);

    mpz_set(pub.e, priv.e);
    mpz_set(pub.n, priv.n);

    return pub;
}

void write_public_key(const public_key &pub, const char* filepath) {
    std::ofstream f(filepath);
    f << "bits" << '\t' << 'e' << '\t' << 'n' << '\n';
    f << MODULUS_SIZE << '\n';
    f << mpz_get_str(NULL, 10, pub.e) << '\n';
    f << mpz_get_str(NULL, 10, pub.n) << '\n';
    f.close();
}

void write_private_key(const private_key &priv, const char* filepath) {
    std::ofstream f(filepath);
    f << 'p' << '\t' << 'q' << '\t' << 'd' << '\t' << 'e' << '\t' << 'L' << '\t' << 'n' << '\n';
    f << mpz_get_str(NULL, 10, priv.p) << '\n';
    f << mpz_get_str(NULL, 10, priv.q) << '\n';
    f << mpz_get_str(NULL, 10, priv.d) << '\n';
    f << mpz_get_str(NULL, 10, priv.e) << '\n';
    f << mpz_get_str(NULL, 10, priv.L) << '\n';
    f << mpz_get_str(NULL, 10, priv.n) << '\n';
    f.close();
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    private_key priv = gen_private_key();
    public_key pub = gen_public_key(priv);
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "Keygen Time: " << std::chrono::duration_cast<std::chrono::microseconds>((stop-start)).count() << '\n';
    write_public_key(pub, "public.txt");
    write_private_key(priv, "private.txt");

    return 0;
}
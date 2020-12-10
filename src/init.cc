// gcc program_name.c -lgmp
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "utils.cc"
/*
OK  here is how RSA works

Message: HELLO
Public: encryption:  keys = (5, 14)
Private: Decryption: keys = (11, 14)

Lets say that HELLO encodes to the number 2 somehow (math)

( Message ^ 5 ) mod 14 = 4       4 = encrypted message

( 4 ^ 11 ) mod 14 = 2       2 = decrypted message

so how are (5, 14) realted to (11, 14) ?

pick two prime numbers: 2, 7
p = 2
q = 7

n = p*q = 14

now make a list between 1 and 14 and remove the common factors, the length of that list is 6
the easy way to get L = 6 is

L = (q-1)*(p-1) = 6 = L


now we get to pick the enxryption key
in the example it was (5, 14) and we know that 14 is the modulus = n

the enctyption key must be between 1 and L = 6
so that cuts it to [2, 3, 4, 5]

It also has to be coprime with L=6 and the modulus=n=14, so the answer has to be 5 in this case

So now we have (5, 14) = (e, n)

Now onto the decryption part, how did we pick (11, 14)
14 is the modulus but where is 11 from?

lets call 11 d, 11 = d

d has to follow one rule:
(d*e) mod (L) = 1
so D can be any number of those numbers that fits the conditions


so symbolically

MESSAGE: m
public_key: (e, n)
private_key: (d, n)

ciphertext = c = (m^e) mod n

m = (c^d) mod n
(d*e) mod L = 1
L = (p-1)(q-1)
n = pq
*/
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
    // Calculate d (multiplicative inverse of e mod phi)
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
    private_key priv = gen_private_key();
    public_key pub = gen_public_key(priv);
    
    write_public_key(pub, "public.txt");
    write_private_key(priv, "private.txt");

    return 0;
}
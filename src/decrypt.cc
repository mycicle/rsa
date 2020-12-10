#include <gmp.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

typedef struct {
    mpz_t p; // p
    mpz_t q; // q
    mpz_t L; // (p-1)*(q-1)
    mpz_t n; // p*q
    mpz_t d; // d*e mod L = 1
    mpz_t e; // public exponent
} private_key;

int main() {
    std::ifstream priv("private.txt");
    std::vector<std::string> priv_info;
    std::string line;

    private_key pk;
    mpz_init(pk.d);
    mpz_init(pk.n);

    // mpz_t d;
    // mpz_t decrypted_message;
    // mpz_t n;
    // mpz_init(d);
    // mpz_init(decrypted_message);
    // mpz_init(n);

    while(getline(priv, line)) {
        priv_info.push_back(line);
    }
    mpz_set_str(pk.d, priv_info[3].c_str(), 10);
    mpz_set_str(pk.n, priv_info[6].c_str(), 10);

    std::ifstream f("message.txt");
    std::vector<std::string> message;

    mpz_t m;
    mpz_t c;
    mpz_init(m);
    mpz_init(c);

    while(getline(f, line)) {
        mpz_set_str(c, line.c_str(), 10);
        // std::cout << "c " << mpz_get_str(NULL, 10, c) << std::endl;
        mpz_powm(m, c, pk.d, pk.n);
        message.push_back(mpz_get_str(NULL, 10, m));
    }

    for (std::string character : message) {
        std::cout << char(stoi(character));
    }
    std::cout << std::endl;
}
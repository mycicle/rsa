#include <gmp.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "utils.cc"

void get_private_key(private_key &pk, const char* filepath) {
    std::ifstream priv("private.txt");
    std::vector<std::string> priv_info;
    std::string line;

    while(getline(priv, line)) {
        priv_info.push_back(line);
    }
    mpz_set_str(pk.d, priv_info[3].c_str(), 10);
    mpz_set_str(pk.n, priv_info[6].c_str(), 10);
}

void decrypt(mpz_t &c, mpz_t &m, private_key &pk) {
    mpz_powm(m, c, pk.d, pk.n);
}

std::vector<std::string> decrypt_message(private_key &pk, const char* filepath) {
    std::ifstream f("message.txt");
    std::vector<std::string> message;
    std::string line;

    mpz_t m;    
    mpz_t c;
    mpz_init(m);
    mpz_init(c);

    while(getline(f, line)) {
        mpz_set_str(c, line.c_str(), 10);
        decrypt(c, m, pk);
        message.push_back(mpz_get_str(NULL, 10, m));
    }

    return message;
}

int main() {
    private_key pk;
    mpz_init(pk.d);
    mpz_init(pk.n);

    get_private_key(pk, "private.txt");
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> message = decrypt_message(pk, "message.txt");
    auto stop = std::chrono::high_resolution_clock::now();

    for (std::string character : message) {
        std::cout << char(stoi(character));
    }
    std::cout << "\nDecrypt Time: " << std::chrono::duration_cast<std::chrono::microseconds>((stop-start)).count() << '\n';
    std::cout << std::endl;
}
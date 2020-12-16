#include <string>
#include <vector>
#include <gmp.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <chrono>

#include "utils.cc"
using namespace std;
/*
    need to do 
    string message => numerical message

    c = m^e mod n
*/
typedef struct {
    unsigned int bits;
    unsigned int e;
    std::string n;
} public_information;

public_information get_pubilc_info(const char* filepath) {
    public_information output;
    ifstream f(filepath);

    vector<string> lines;
    int counter = 0;

    for(int i = 0; i < 4; i++) {
        string line = "";
        getline(f, line);
        lines.push_back(line);
    }

    output.bits = stoi(lines[1]);
    output.e = stoi(lines[2]);
    output.n = lines[3];

    return output;
}

void encrypt(mpz_t &c, mpz_t &m, public_key &pub) {
    mpz_powm(c, m, pub.e, pub.n);
}

int main(int argc, char *argv[]) {

    public_information info = get_pubilc_info("public.txt");
    const unsigned int mod_size = info.bits;
    const unsigned int block_size = (mod_size/8);
    
    public_key pub;
    mpz_init(pub.e);
    mpz_init(pub.n);

    mpz_set_ui(pub.e, info.e);
    mpz_set_str(pub.n, info.n.c_str(), 10);
  
    mpz_t c;
    mpz_t m;
    mpz_init(c);
    mpz_init(m);

    char *message = argv[1];
    // char message[] = "Hello there, GENERAL KENOBI!";
    int length = strlen(message);
    auto start = std::chrono::high_resolution_clock::now();
    vector<string> ciphertext;
    for (int i = 0; i < length; i++) {

        mpz_set_ui(m, message[i]);
        mpz_powm(c, m, pub.e, pub.n);
        ciphertext.push_back(mpz_get_str(NULL, 10, c));
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::ofstream f("message.txt");
    for (string line : ciphertext) {
        f << line << '\n';
    }

    std::cout << "Encrypt Time: " << std::chrono::duration_cast<std::chrono::microseconds>((stop-start)).count() << '\n';
}
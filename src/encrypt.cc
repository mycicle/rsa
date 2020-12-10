#include <string>
#include <vector>
#include <gmp.h>
#include <fstream>
#include <iostream>
#include <string.h>

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

typedef struct {
    mpz_t e;    // public exponent
    mpz_t n;    // modulus
} public_key; 

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
    const unsigned int MODULUS_SIZE = info.bits;
    const unsigned int BLOCK_SIZE = (MODULUS_SIZE/8);
    
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
    vector<string> ciphertext;
    for (int i = 0; i < length; i++) {

        mpz_set_ui(m, message[i]);
        mpz_powm(c, m, pub.e, pub.n);
        ciphertext.push_back(mpz_get_str(NULL, 10, c));
    }

    std::ofstream f("message.txt");
    for (string line : ciphertext) {
        f << line << '\n';
    }

    // mpz_import(m, length, 1, sizeof(ciphertext[0]), 0, 0, message);
    // // cout << mpz_get_str(NULL, 10, m) << endl;

    // encrypt(c, m, pub);

    // mpz_export(ciphertext, NULL, 1, sizeof(ciphertext[0]), 0, 0, c);

}

// int main() {

//     ifstream f("public.txt");

//     vector<string> lines;
//     int counter = 0;

//     for(int i = 0; i < 3; i++) {
//         string line = "";
//         if (i == 0) {
//             getline(f, line);
//             continue;
//         }
//         getline(f, line);
//         lines.push_back(line);
//     }

//     const unsigned int bits = stoi(lines[0]);
//     const unsigned int e1 = stoi(lines[1]);

//     cout << bits << endl;
//     cout << e1 << endl;

//     const char* input = "Hello there, GENERAL KENOBI!";
//     mpz_t e;
//     mpz_t m;
//     mpz_init(e);
//     mpz_init(m);

//     mpz_set_ui(e, e1);
//     mpz_set_ui(m);
// }
#include "boost/multiprecision/cpp_int.hpp"

typedef boost::multiprecision::int512_t int512_t;
typedef boost::multiprecision::int1024_t int1024_t;
// https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
// Returns modulo inverse of a with respect 
// to m using extended Euclid Algorithm 
// Assumption: a and m are coprimes, i.e., 
// gcd(a, m) = 1 
int1024_t modInverse(int512_t a, int1024_t m) { 
    int1024_t m0 = m; 
    int1024_t y = 0;
    int1024_t x = 1; 
    int1024_t a1 = (int1024_t) a;
    if (m == 1) 
        return 0; 
  
    while (a > 1) { 
        // q is quotient 
        int1024_t q = a1 * (1/m);
        int1024_t t = m; 
  
        // m is remainder now, process same as 
        // Euclid's algo 
        m = a1 % m, a1 = t; 
        t = y; 
  
        // Update y and x 
        y = x - q * y; 
        x = t; 
    } 
  
    // Make x positive 
    if (x < 0) 
        x += m0; 
    
    return (int1024_t) x;
}
    
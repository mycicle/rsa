#include "boost/multiprecision/cpp_int.hpp"
#include <boost/random.hpp>
#include <boost\multiprecision\miller_rabin.hpp>
typedef boost::multiprecision::uint512_t uint512_t;
typedef boost::random::independent_bits_engine<boost::random::mt19937, 512, uint512_t> generator512;
generator512 gen512;



uint512_t genprime(int numBits) {
    for(int i = 0; i < 10000; i ++){
        uint512_t ret = gen512();
        if(boost::multiprecision::miller_rabin_test(ret, 25)){
            return ret;
        }
    }
}

int main(){

}
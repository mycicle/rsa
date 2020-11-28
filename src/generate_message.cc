#ifndef GENERATE_MESSAGE
#define GENERATE_MESSAGE

#include <iostream>
#include <memory.h>
#include "boost/multiprecision/cpp_int.hpp"

typedef boost::multiprecision::int1024_t int1024_t;

int1024_t generate_message(const char* input, int length, int max_bit_length=1024) {
    /*
    With a cstring input of length max_bit_length/8 or less (ex: 128 ASCII characters for a max_bit_length of 1024)
    pad the message to the required length (again, 128 for mbl of 1024) and then store that into an mbl precision number 
    (ex. a 1024 bit number) so that we can continue with the encryption process
    */
    int max_byte_length = max_bit_length/8;
    if (length > max_byte_length)
        throw std::out_of_range("message too long, maximum number of letters per message = max_bit_length/8");
    namespace mp = boost::multiprecision;
    
    if (length < max_byte_length) {
        // we want to pad out the input char array with 0x0 so we can convert to the proper integer length later
        char* temp = new char[max_byte_length];
        memcpy(temp, input, length);
        int additions = max_byte_length - length;
        for (char* p = temp+length; p < temp+max_byte_length; p) {
            char pad = 0x0;
            (*p) &= pad;
        }
        input = temp;
    }
    
    int1024_t base = 0;
    for (int i = max_byte_length-1, j=0; i >= 0; i--, j++) {
        int offset = i*8;
        int temp = ((int)input[j]) << offset;
        base |= temp;
    }

    return base;
}

#endif
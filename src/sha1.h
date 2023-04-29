#ifndef SHA_SHA1_H
#define SHA_SHA1_H

#include <iostream>

class SHA1 {
    public:
        const static uint32_t entryConstants[80];
        const static uint32_t hashConstants[5];
    
        static uint32_t rotl(uint32_t x, uint32_t n);
};

std::string sha1(std::string input);

#endif
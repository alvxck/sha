#ifndef SHA_SHA1_H
#define SHA_SHA1_H

#include <iostream>

class SHA1 {
    private:
        const static uint32_t entry_constants[80];
        const static uint32_t hash_constants[5];

        uint32_t rotl(uint32_t x, uint32_t n);
};

std::string sha1(std::string input);

#endif
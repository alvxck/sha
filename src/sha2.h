#ifndef SHA_SHA2_H
#define SHA_SHA2_H

#include <iostream>

class SHA2 {
    private: 
        const static uint64_t round_constants_x64[80];
        const static uint32_t round_constants_x32[64];
        const static uint64_t hash_constants[6][8];

        uint32_t rotr(uint32_t x, uint32_t n);

    public:
        SHA2();
        ~SHA2();

        std::string sha224(std::string input);
        std::string sha256(std::string input);
        std::string sha384(std::string input);
        std::string sha512(std::string input);
        std::string sha512_224(std::string input);
        std::string sha512_256(std::string input);

};

#endif
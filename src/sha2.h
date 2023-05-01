#ifndef SHA_SHA2_H
#define SHA_SHA2_H

#include <iostream>

class SHA2 {
    private:
        std::string input;
        std::string hash; 

    public: 
        const static uint64_t round_constants_x64[80];
        const static uint32_t round_constants_x32[64];
        const static uint64_t hash_constants[6][8];

        SHA2(std::string);
        
        void hash32(uint8_t bit_length);
        void hash64(uint8_t bit_length);
        
        uint32_t rotr32(uint32_t x, uint32_t n);
        uint64_t rotr64(uint64_t x, uint64_t n);
        std::string getHash();
};

std::string sha224(std::string input);
std::string sha256(std::string input);
std::string sha384(std::string input);
std::string sha512(std::string input);
std::string sha512_224(std::string input);
std::string sha512_256(std::string input);

#endif
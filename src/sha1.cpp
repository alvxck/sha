#include "sha1.h"
#include <vector>
#include <iostream>
#include <bitset>
#include <string>
#include <cmath>


const uint32_t SHA1::entryConstants[80] = {
    0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 
    0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 
    0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 
    0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 
    0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 
    0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 
    0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 
    0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 
    0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 
    0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6 
};

const uint32_t SHA1::hashConstants[5] = {
    0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0
};

uint32_t SHA1::rotl(uint32_t x, uint32_t n) {
    return fmod((x << n) | (x >> (32 - n)), pow(2, 32));
};

std::string sha1(std::string input) {
    // Pre-process input bits
    uint32_t inputLength = input.length() * 8;
    std::string raw;

    for (int i = 0; i < inputLength / 8; i++) {
        raw += std::bitset<8>(input[i]).to_string();
    }   

    raw += "1";

    if (inputLength % 512 < 448) {
        std::string padding = std::bitset<512>(inputLength).to_string();
        raw += padding.substr((inputLength % 512) + 1, 512);
    } else {
        std::string padding = std::bitset<1024>(inputLength).to_string();
        raw += padding.substr((inputLength % 1024) + 1, 1024);
    }

    // Initialize chunks
    std::vector<uint32_t*> chunks;

    for (int i = 0; i < raw.length() / 512; i++) {
        uint32_t chunk[80] = {0};

        for (int j = 0; j < 16; j++) {
            chunk[j] = std::stoi(raw.substr(j*32, 32), nullptr, 2);
        }

        chunks.push_back(chunk);
    }

    // Compress chunks
    for (int i = 0; i < chunks.size(); i++) {
        for (int j = 16; j < 80; j++) {
            chunks[i][j] = fmod(SHA1::rotl(chunks[i][j-3] ^ chunks[i][j-8] ^ chunks[i][j-14] ^ chunks[i][j-16], 1), pow(2, 32));
        }
    }

    // Mutate chunks

    // Construct hash


    return input;
}
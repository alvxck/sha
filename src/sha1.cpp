#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <cmath>
#include <sstream>
#include "sha1.h"

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

    uint32_t hashProcessing[5] = {
        0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0
    };

    // Mutate chunks
    for (int i = 0; i < chunks.size(); i++) {
        uint32_t a = SHA1::hashConstants[0];
        uint32_t b = SHA1::hashConstants[1];
        uint32_t c = SHA1::hashConstants[2];
        uint32_t d = SHA1::hashConstants[3];
        uint32_t e = SHA1::hashConstants[4];

        for (int j = 0; j < 80; j++) {
            uint32_t f = 0;
            uint32_t k = SHA1::entryConstants[j];

            if (j < 20) {
                f = (b & c) | ((~b) & d);
            } 
            if (j < 40) {
                f = b ^ c ^ d;
            } 
            if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
            }
            if (j < 80) {
                f = b ^ c ^ d;
            }

            uint32_t temp = fmod(SHA1::rotl(a, 5) + f + e + k + chunks[i][j], pow(2, 32));
            e = d;
            d = c;
            c = SHA1::rotl(b, 30);
            b = a;
            a = temp;
        }

        hashProcessing[0] = fmod(hashProcessing[0] + a, pow(2, 32));
        hashProcessing[1] = fmod(hashProcessing[1] + b, pow(2, 32));
        hashProcessing[2] = fmod(hashProcessing[2] + c, pow(2, 32));
        hashProcessing[3] = fmod(hashProcessing[3] + d, pow(2, 32));
        hashProcessing[4] = fmod(hashProcessing[4] + e, pow(2, 32));
    }

    // Construct hash
    std::string output = "";

    for (int i = 0; i < 5; i++) {
        std::stringstream ss;
        ss << std::hex << hashProcessing[i];
        output += ss.str();
    }

    return output;
}
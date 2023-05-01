#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <cmath>
#include <sstream>
#include "sha1.h"

SHA1::SHA1(std::string input) {
    this->input = input;
}

uint32_t SHA1::rotl(uint32_t x, uint32_t n) {
    return fmod((x << n) | (x >> (32 - n)), pow(2, 32));
};

void SHA1::hash32() {
    // Pre-process input bits
    uint32_t inputLength = this->input.length() * 8;
    std::string raw;

    for (int i = 0; i < inputLength / 8; i++) {
        raw += std::bitset<8>(this->input[i]).to_string();
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
            chunk[j] = std::stoul(raw.substr(j*32, 32), nullptr, 2);
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
    for (int i = 0; i < chunks.size(); i++) {
        uint32_t a = this->hash_constants[0];
        uint32_t b = this->hash_constants[1];
        uint32_t c = this->hash_constants[2];
        uint32_t d = this->hash_constants[3];
        uint32_t e = this->hash_constants[4];

        for (int j = 0; j < 80; j++) {
            uint32_t f = 0;
            uint32_t k = this->entry_constants[j];

            if (j < 20) {
                f = (b & c) | ((~b) & d);
            } else if (j < 40) {
                f = b ^ c ^ d;
            } else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
            }
            else {
                f = b ^ c ^ d;
            }

            uint32_t temp = fmod(SHA1::rotl(a, 5) + f + e + k + chunks[i][j], pow(2, 32));
            e = d;
            d = c;
            c = SHA1::rotl(b, 30);
            b = a;
            a = temp;
        }

        this->hash_constants[0] = fmod(this->hash_constants[0] + a, pow(2, 32));
        this->hash_constants[1] = fmod(this->hash_constants[1] + b, pow(2, 32));
        this->hash_constants[2] = fmod(this->hash_constants[2] + c, pow(2, 32));
        this->hash_constants[3] = fmod(this->hash_constants[3] + d, pow(2, 32));
        this->hash_constants[4] = fmod(this->hash_constants[4] + e, pow(2, 32));
    }

    // Construct hash
    for (int i = 0; i < 5; i++) {
        std::stringstream ss;
        ss << std::hex << this->hash_constants[i];
        this->hash += ss.str();
    }
}

std::string SHA1::getHash() {
    return this->hash;
}

std::string sha1(std::string input) {
    SHA1 *newHash = new SHA1(input);
    newHash->hash32();
    return newHash->getHash();
}
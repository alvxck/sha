#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <cmath>
#include <sstream>
#include "sha2.h"

SHA2::SHA2(std::string input) {
    this->input = input;
}

uint32_t SHA2::rotr32(uint32_t x, uint32_t n) {
    return fmod((x >> n) | (x << (32 - n)), pow(2, 32));
};

uint64_t SHA2::rotr64(uint64_t x, uint64_t n) {
    return fmod((x >> n) | (x << (64 - n)), pow(2, 64));
};

void SHA2::hash32(uint8_t bit_length, uint32_t *hash_constants) {
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
        uint32_t chunk[64] = {0};

        for (int j = 0; j < 16; j++) {
            chunk[j] = std::stoi(raw.substr(j*32, 32), nullptr, 2);
        }

        chunks.push_back(chunk);
    }

    // Compress chunks
    for (int i = 0; i < chunks.size(); i++) {
        for (int j = 16; j < 64; j++) {
            uint32_t s0 = SHA2::rotr32(chunks[i][j-15], 7) ^ SHA2::rotr32(chunks[i][j-15], 18) ^ chunks[i][j-15] >> 3;
            uint32_t s1 = SHA2::rotr32(chunks[i][j-2], 17) ^ SHA2::rotr32(chunks[i][j-2], 19) ^ chunks[i][j-2] >> 10;
            chunks[i][j] = fmod(chunks[i][j-16] + s0 + chunks[i][j-7] + s1, pow(2, 64));
        }
    }

    // Mutate chunks
    for (int i = 0; i < chunks.size(); i++) {
        uint32_t a = hash_constants[0];
        uint32_t b = hash_constants[1];
        uint32_t c = hash_constants[2];
        uint32_t d = hash_constants[3];
        uint32_t e = hash_constants[4];
        uint32_t f = hash_constants[5];
        uint32_t g = hash_constants[6];
        uint32_t h = hash_constants[7];

        for (int j = 0; j < 64; j++) {
            uint32_t s0 = SHA2::rotr32(e, 16) ^ SHA2::rotr32(e, 11) ^ SHA2::rotr32(e, 25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t temp1 = fmod(h + s0 + ch + SHA2::round_constants_x32[j] + chunks[i][j], pow(2, 32));
            uint32_t s1 = SHA2::rotr32(a, 2) ^ SHA2::rotr32(a, 13) ^ SHA2::rotr32(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b ^ c);
            uint32_t temp2 = fmod(s1 + maj, pow(2, 32));

            h = g;
            g = f;
            f = e;
            e = fmod(d + temp1, pow(2, 32));
            d = c;
            c = b;
            b = a;
            a = fmod(temp1 + temp2, pow(2, 32));
        }

        hash_constants[0] = fmod(hash_constants[0] + a, pow(2, 32));
        hash_constants[1] = fmod(hash_constants[1] + b, pow(2, 32));
        hash_constants[2] = fmod(hash_constants[2] + c, pow(2, 32));
        hash_constants[3] = fmod(hash_constants[3] + d, pow(2, 32));
        hash_constants[4] = fmod(hash_constants[4] + e, pow(2, 32));
        hash_constants[5] = fmod(hash_constants[5] + f, pow(2, 32));
        hash_constants[6] = fmod(hash_constants[6] + g, pow(2, 32));
        hash_constants[7] = fmod(hash_constants[7] + h, pow(2, 32));
    }

    for (int i = 0; i < 7; i++) {
        std::stringstream ss;
        ss << std::hex << hash_constants[i];
        this->hash += ss.str();
    }
};

void SHA2::hash64(uint8_t bit_length, uint64_t *hash_constants) {
    
};

std::string SHA2::getHash() {
    return this->hash;
}

std::string sha224(std::string input) {
    uint32_t hash_constants[8] = {
        0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
    };

    SHA2 *newHash = new SHA2(input);
    newHash->hash32(224, hash_constants);

    return newHash->getHash();
};

// std::string sha256(std::string input) {
// };

// std::string sha384(std::string input) {
// };

// std::string sha512(std::string input) {
// };

// std::string sha512_224(std::string input) {
// };

// std::string sha512_256(std::string input) {
// };
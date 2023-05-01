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

uint32_t SHA2::rotr32(uint32_t x, uint8_t n) {
    return fmod((x >> n) | (x << (32 - n)), pow(2, 32));
};

uint64_t SHA2::rotr64(uint64_t x, uint8_t n) {
    return fmod((x >> n) | (x << (64 - n)), pow(2, 64));
};

void SHA2::hash32(uint16_t bit_length, uint32_t *hash_constants) {
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
        uint32_t chunk[64] = {0};

        for (int j = 0; j < 16; j++) {
            chunk[j] = std::stoi(raw.substr(j*32, 32), nullptr, 2);
        }

        chunks.push_back(chunk);
    }

    // Compress chunks
    for (int i = 0; i < chunks.size(); i++) {
        for (int j = 16; j < 64; j++) {
            uint32_t s0 = SHA2::rotr32(chunks[i][j-15], 7) ^ SHA2::rotr32(chunks[i][j-15], 18) ^ (chunks[i][j-15] >> 3);
            uint32_t s1 = SHA2::rotr32(chunks[i][j-2], 17) ^ SHA2::rotr32(chunks[i][j-2], 19) ^ (chunks[i][j-2] >> 10);
            chunks[i][j] = fmod(chunks[i][j-16] + s0 + chunks[i][j-7] + s1, pow(2, 32));
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
            uint32_t s0 = (SHA2::rotr32(e, 6)) ^ (SHA2::rotr32(e, 11)) ^ (SHA2::rotr32(e, 25));
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t temp1 = fmod(h + s0 + ch + SHA2::round_constants_x32[j] + chunks[i][j], pow(2, 32));
            uint32_t s1 = (SHA2::rotr32(a, 2)) ^ (SHA2::rotr32(a, 13)) ^ (SHA2::rotr32(a, 22));
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
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

    for (int i = 0; i < 8; i++) {
        std::stringstream ss;
        ss << std::hex << hash_constants[i];
        this->hash += ss.str();
    }
};

void SHA2::hash64(uint16_t bit_length, uint64_t *hash_constants) {
    // Pre-process input bits
    uint32_t inputLength = this->input.length() * 8;
    std::string raw;

    for (int i = 0; i < inputLength / 8; i++) {
        raw += std::bitset<8>(this->input[i]).to_string();
    }   

    raw += "1";

    if (inputLength % 1024 < 896) {
        std::string padding = std::bitset<1024>(inputLength).to_string();
        raw += padding.substr((inputLength % 1024) + 1, 1024);
    } else {
        std::string padding = std::bitset<2048>(inputLength).to_string();
        raw += padding.substr((inputLength % 2048) + 1, 2048);
    }

    // Initialize chunks
    std::vector<uint64_t*> chunks;

    for (int i = 0; i < raw.length() / 1024; i++) {
        uint64_t chunk[80] = {0};

        for (int j = 0; j < 16; j++) {
            chunk[j] = std::stoul(raw.substr(j*64, 64), nullptr, 2);
        }

        chunks.push_back(chunk);
    }

    // Compress chunks
    for (int i = 0; i < chunks.size(); i++) {
        for (int j = 16; j < 80; j++) {
            uint64_t s0 = SHA2::rotr64(chunks[i][j-15], 1) ^ SHA2::rotr64(chunks[i][j-15], 8) ^ (chunks[i][j-15] >> 7);
            uint64_t s1 = SHA2::rotr64(chunks[i][j-2], 19) ^ SHA2::rotr64(chunks[i][j-2], 61) ^ (chunks[i][j-2] >> 6);
            chunks[i][j] = fmod(chunks[i][j-16] + s0 + chunks[i][j-7] + s1, pow(2, 64));
        }
    }

    // print vector 

    for (int i = 0; i < chunks.size(); i++) {
        for (int j = 0; j < 80; j++) {
            std::cout << chunks[i][j] << " ";
        }
    }

    exit(0);


    // Mutate chunks
    for (int i = 0; i < chunks.size(); i++) {
        uint64_t a = hash_constants[0];
        uint64_t b = hash_constants[1];
        uint64_t c = hash_constants[2];
        uint64_t d = hash_constants[3];
        uint64_t e = hash_constants[4];
        uint64_t f = hash_constants[5];
        uint64_t g = hash_constants[6];
        uint64_t h = hash_constants[7];

        for (int j = 0; j < 80; j++) {
            uint64_t s0 = (SHA2::rotr64(e, 14)) ^ (SHA2::rotr64(e, 18)) ^ (SHA2::rotr64(e, 41));
            uint64_t ch = (e & f) ^ (~e & g);
            uint64_t temp1 = fmod(h + s0 + ch + SHA2::round_constants_x64[j] + chunks[i][j], pow(2, 64));
            uint64_t s1 = (SHA2::rotr64(a, 28)) ^ (SHA2::rotr64(a, 34)) ^ (SHA2::rotr64(a, 39));
            uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint64_t temp2 = fmod(s1 + maj, pow(2, 64));

            h = g;
            g = f;
            f = e;
            e = fmod(d + temp1, pow(2, 64));
            d = c;
            c = b;
            b = a;
            a = fmod(temp1 + temp2, pow(2, 64));
        }

        hash_constants[0] = fmod(hash_constants[0] + a, pow(2, 64));
        hash_constants[1] = fmod(hash_constants[1] + b, pow(2, 64));
        hash_constants[2] = fmod(hash_constants[2] + c, pow(2, 64));
        hash_constants[3] = fmod(hash_constants[3] + d, pow(2, 64));
        hash_constants[4] = fmod(hash_constants[4] + e, pow(2, 64));
        hash_constants[5] = fmod(hash_constants[5] + f, pow(2, 64));
        hash_constants[6] = fmod(hash_constants[6] + g, pow(2, 64));
        hash_constants[7] = fmod(hash_constants[7] + h, pow(2, 64));
    }

    for (int i = 0; i < 8; i++) {
        std::stringstream ss;
        ss << std::hex << hash_constants[i];
        this->hash += ss.str();
    }

    this->hash = this->hash.substr(0, bit_length / 4);
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

std::string sha256(std::string input) {
    uint32_t hash_constants[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    SHA2 *newHash = new SHA2(input);
    newHash->hash32(256, hash_constants);

    return newHash->getHash();
};

std::string sha384(std::string input) {
    uint64_t hash_constants[8] = { 
        0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939, 
        0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4 
    };

    SHA2 *newHash = new SHA2(input);
    newHash->hash64(384, hash_constants);

    return newHash->getHash();
};

std::string sha512(std::string input) {
    uint64_t hash_constants[8] = {
        0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 
        0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x05be0cd19137e2179
    };

    SHA2 *newHash = new SHA2(input);
    newHash->hash64(512, hash_constants);

    return newHash->getHash();
};

std::string sha512_224(std::string input) {
    uint64_t hash_constants[8] = {
        0x8c3d37c819544da2, 0x73e1996689dcd4d6, 0x1dfab7ae32ff9c82, 0x679dd514582f9fcf,
        0x0F6D2B697BD44DA8, 0x77e36f7304c48942, 0x3f9d85a86a1d36c8, 0x1112e6ad91d692a1
    };
    
    SHA2 *newHash = new SHA2(input);
    newHash->hash64(224, hash_constants);

    return newHash->getHash();
};

std::string sha512_256(std::string input) {
    uint64_t hash_constants[8] = {
        0x22312194fc2bf72c, 0x9f555fa3c84c64c2, 0x2393b86b6f53b151, 0x963877195940eabd,
        0x96283ee2a88effe3, 0xbe5e1e2553863992, 0x2b0199fc2c85b8aa, 0x0eb72ddc81c52ca2
    };

    SHA2 *newHash = new SHA2(input);
    newHash->hash64(256, hash_constants);

    return newHash->getHash();
};
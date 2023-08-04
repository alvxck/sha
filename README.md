# SHA

C++ implementation of SHA-1 and SHA-2 Cryptographic Hash Algorithms.

## Overview

* **[SHA-1:](https://github.com/alvxck/SHA/blob/master/src/sha1.cpp)** [ SHA-1 ]

* **[SHA-2:](https://github.com/alvxck/SHA/blob/master/src/sha2.cpp)** [ SHA-224, SHA-256, SHA-384, SHA-512, SHA-512/224, SHA-512/256 ]

## Getting Started

<!-- SHA1 -->
### SHA-1 Hash Algorithm
Implementation in accordance with NIST **[FIPS PUB 180-4](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)** standard.

#### [sha1.cpp](https://github.com/alvxck/SHA/blob/master/src/sha1.cpp)
```
sha1('hello world')
> '2aae6c35c94fcfb415dbe95f408b9ce91ee846ed'
```

#### Hashing Process

1. Pre Process
    * Convert each character from `message` to 8-bits and append to `data`. Then append a single *1*.
    * Pad `data` with *0's* until it is a multiple of 512-bits.
    * Use the last 64-bits of `data` to represent the length of `message`.

2. Chunk Loop
    * Break `data` into 32-bit sections and save all sections to `blocks`.
    * Append 64 *0's* to `blocks`.

3. Compression Loop
    * Modify the *0's* added in step 2 following SHA-1 hash computation procedures. **[[6.1.0]](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf#page=23)**.

4. Mutation Loop
    * Initialize all 8 `hash_constants` to a b c d e respectively.
    * Modify a-e using the values in `blocks` and values in `round_constants` following SHA-1 hash computation procedures. **[[6.1.0]](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf#page=23)**.
    * Repeat previous step once for each value in `round_constants`.  
    (x80 iterations)

6. Concatenation
    * Concatenate newly modified values in `hash_constants` to create digest.
    * Return digest to user.

<!-- SHA2 -->
### SHA-2 Hash Algorithms

Implementations in accordance with NIST **[FIPS PUB 180-4](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)** standard.

#### [sha2.cpp](https://github.com/alvxck/SHA/blob/master/src/sha2.cpp)
```
sha224('hello world') 
> '2f05477fc24bb4faefd86517156dafdecec45b8ad3cf2522a563582b'

sha256('hello world')
> 'b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9'

sha384('hello world')
> 'fdbd8e75a67f29f701a4e040385e2e23986303ea10239211af907fcbb83578b3e417cb71ce646efd0819dd8c088de1bd'

sha512('hello world')
> '309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee511a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670da34255b45b0cfd830e81f605dcf7dc5542e93ae9cd76f'

sha512_224('hello world')
> '22e0d52336f64a998085078b05a6e37b26f8120f43bf4db4c43a64ee'

sha512_256('hello world')
> '0ac561fac838104e3f2e4ad107b4bee3e938bf15f2b15f009ccccd61a913f017'
```

#### Hashing Process
* 32-bit [ SHA-224, SHA-256 ] 
* 64-bit [ SHA-384, SHA-512, SHA-512/224, SHA-512/256 ]

1. Pre Process
    * Convert each character from `message` to 8-bits and append to `data`. Then append a single *1*.
    * Pad `data` with *0's* until it is a multiple of 512-bits (32-bit) or a multiple of 1024-bits (64-bit).
    * Use the last 64-bits (32-bit) or 128-bits (64-bit) of `data` to represent the length of `message`.

2. Chunk Loop
    * Break `data` into 32-bit (32-bit) or 64-bit (64-bit) sections and save all sections to `blocks`.
    * Append 48 *0's* (32-bit) or 80 *0's* (64-bit) to `blocks`.

3. Compression Loop
    * Modify the *0's* added in step 2 following SHA-2 hash computation procedures.  
    (32-bit **[[6.2.2]](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf#page=27)**. 64-bit **[[6.4.2]](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf#page=29)**)

4. Mutation Loop
    * Initialize all 8 `hash_constants` to a b c d e f g h respectively.
    * Modify a-h using the values in `blocks` and values in `round_constants` following SHA-2 hash computation procedures.  
    (32-bit **[[6.2.2]](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf#page=27)**. 64-bit **[[6.4.2]](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf#page=29)**)
    * Repeat previous step once for each value in `round_constants`.  
    (32-bit: x64 iterations | 64-bit: x80 iterations)

6. Concatenation
    * Concatenate newly modified values in `hash_constants` to create digest.
    * Return digest to user.

#include <iostream>
#include "sha2.h"

int main() {
    std::string input = "hello world";

    std::cout << sha512_256(input) << std::endl;

    return 0;
}
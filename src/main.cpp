#include <iostream>
#include "sha2.h"

int main() {
    std::string input = "Hello, world!";

    std::cout << sha224(input) << std::endl;

    return 0;
}
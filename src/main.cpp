#include <iostream>
#include "sha2.h"

int main() {
    std::string input = "hello world";

    std::cout << sha384(input) << std::endl;

    return 0;
}
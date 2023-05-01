#include <iostream>
#include "sha1.h"

int main() {
    std::string input = "Hello, world!";

    std::cout << sha1(input) << std::endl;

    return 0;
}
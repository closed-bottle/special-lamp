#include <iostream>

#include "../lampString.h"

int main(int argc, const char * argv[]) {
    std::cout << "Hello World!\n";

    Lamp::String lampString = Lamp::String("Hello World!");

    std::cout << lampString.c_str() << std::endl;

    return 0;
}
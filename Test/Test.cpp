#include <iostream>

#include "../lampString.h"
#include "../lampVector.h++"

#include <vector>
#include <chrono>

int main(int argc, const char * argv[]) {
    std::cout << "Hello World!\n";

    {
        Lamp::Vector<Lamp::String> lampVector1 = Lamp::Vector<Lamp::String>();

        {
            Lamp::String lampString1 = Lamp::String("First");
            lampString1 = Lamp::String("Second");
            lampVector1.push_back(std::move(lampString1));
        }


        std::cout << "----" << std::endl;

        std::cout << lampVector1[0].c_str() << std::endl;
    }

    return 0;
}
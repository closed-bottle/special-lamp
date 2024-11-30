#include <iostream>

#include "../lampString.h"

int main(int argc, const char * argv[]) {
    std::cout << "Hello World!\n";

    Lamp::String lampString = Lamp::String("Hello World!");

    std::cout << lampString.c_str() << std::endl;
    Lamp::String lampString1 = lampString;
    lampString1 = Lamp::String("Refresh");
    lampString1 = "C style string";
    std::cout << "Same or not : " << (lampString1 == "C style string") << std::endl;
    //Lamp::String lampString2 = Lamp::String("First");
    //Lamp::String lampString3 = Lamp::String("Seocnd");
    //Lamp::String lampString4 = Lamp::String("Third");

    //Lamp::Vector<Lamp::String> lampVector1 = Lamp::Vector<Lamp::String>();

    //lampVector1.push_back(lampString2);
    //lampVector1.push_back(lampString3);
    //lampVector1.push_back(lampString4);

    return 0;
}
#include <iostream>

#include "../lampString.h"
#include "../lampVector.h++"
#include "../lampCRC32.h++"
#include "../lampMurmur3.h++"

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

    char c[] = "123456jkjpj30223l789";
    Lamp::CRC32 crc32;
    Lamp::LampMurmur3 murmur;


    // 1 should be 0x83DCEFB7
    // 123456789 should be 89A1897F (Normal)
    // 0xCBF43926 (Reflected)

    //crc32.GenerateCRC32LUT();
    //crc32.OutputTable();
    uint64_t murmur_result[2] = {0,0};
    murmur.Murmur3(reinterpret_cast<uint8_t *>(c), sizeof(c) - 1, &murmur_result);
    std::cout << std::hex << crc32.GetCRC32(reinterpret_cast<uint8_t *>(c), sizeof(c) - 1) << std::endl;
    std::cout << std::hex << murmur_result[0] << murmur_result[1] << std::endl;

    return 0;
}
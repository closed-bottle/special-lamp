#include <iostream>

#include "../lampString.h"
#include "../lampVector.h++"
#include "../lampCRC32.h++"
#include "../lampMurmur3.h++"
#include "../lampList.h++"
#include "../lampUnordered_map.h++"

#include <vector>
#include <chrono>

template<typename T>
void PrintList(Lamp::list<T> & _list) {
    while (!_list.empty()) {
        std::cout << _list.front() << ", ";
        _list.pop_front();
    }
}

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

    /*
    Lamp::list<uint32_t> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    list.push_back(7);
    list.push_back(8);
    list.push_back(9);

    std::cout << "--------------" << std::endl;
    PrintList(list);
    std::cout << "--------------" << std::endl;

    list.insert(0, 0);
    list.insert(1, 1);
    list.insert(2, 2);
    list.insert(2, 3);
    list.insert(2, 4);
    list.insert(2, 5);
    PrintList(list);
    */
    auto str1 = Lamp::String("First");
    auto str2 = Lamp::String("Second");
    auto str3 = Lamp::String("Third");
    auto strtest = Lamp::String("First");
    auto str123 = Lamp::String("123456789");


    Lamp::unordered_map<Lamp::String, uint32_t> map;
    map[str123] = 123;

    map[str1] = 1;
    map[str2] = 2;
    map[str3] = 3;
    map["??"] = 88;



    return 0;
}
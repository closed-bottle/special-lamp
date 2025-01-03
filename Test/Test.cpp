#include <iostream>
#include <io.h>

#include "../lampString.h++"
#include "../lampVector.h++"
#include "../lampCRC32.h++"
#include "../lampMurmur3.h++"
#include "../lampList.h++"
#include "../lampUnordered_map.h++"
#include "../lampMath.h++"

#include <vector>
#include <chrono>
#include <random>


template<typename T>
void PrintList(Lamp::list<T> & _list) {
    while (!_list.empty()) {
        std::cout << _list.front() << ", ";
        _list.pop_front();
    }
}

int main(int argc, const char * argv[]) {

    std::cout << "Hello World!\n";
    /*
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
    */

    {
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


        auto str1 = Lamp::String("First");
        auto str2 = Lamp::String("Second");
        auto str3 = Lamp::String("Third");
        auto strtest = Lamp::String("First");
        auto str123 = Lamp::String("123456789");

        std::cout << str1.c_str() << std::endl;
        std::cout << str2.c_str() << std::endl;
        std::cout << str3.c_str() << std::endl;
        std::cout << strtest.c_str() << std::endl;
        std::cout << str123.c_str() << std::endl;

        Lamp::unordered_map<Lamp::String, uint32_t> map;
        map[str123] = 123;

        map[str1] = 1;
        map[str2] = 2;
        map[str3] = 3;
        map["??"] = 88;

        std::cout << std::dec;
        std::cout << "str123 = " << map[str123] << std::endl;
        std::cout << "str1   = " << map[str1] << std::endl;
        std::cout << "str2   = " << map[str2] << std::endl;
        std::cout << "str3   = " << map[str3] << std::endl;
        std::cout << "??     = " << map["??"] << std::endl;
        std::cout << "random = " << map["random"] << std::endl;

        Lamp::Vector<uint32_t> rand_number_collection;
        Lamp::Vector<Lamp::String> rand_str_collection;


        // Initially tried to use C++11 random, but it causes too many issues to external debugger.

        const char rand_str[27] = "acdefghijklmnopqrstuvwxyz";

        uint64_t t = 0xFFFF;


        for (int i = 0; i < t; ++i) {
            uint32_t uu = (uint32_t)rand() * 123456;
            rand_number_collection.push_back(uu);
        }


        for (int i = 0; i < t; ++i) {

            Lamp::String rng = "12345";
            rng[0] = rand_str[rand() % 27];
            rng[1] = rand_str[rand() % 27];
            rng[2] = rand_str[rand() % 27];
            rng[3] = rand_str[rand() % 27];
            rng[4] = rand_str[rand() % 27];

            rand_str_collection.push_back(rng);
        }


        for (int i = 0; i < t; ++i) {
            if (map.find(rand_str_collection[i])) {
                rand_number_collection[i] = map[rand_str_collection[i]];
            }
            else {
                map[rand_str_collection[i]] = rand_number_collection[i];
            }

        }

        for (int i = 0; i < t; ++i) {
            uint32_t left = map[rand_str_collection[i]];
            uint32_t right = rand_number_collection[i];
            if (left != right)
                std::cout << "mismatch found at : " << i << std::endl;
        }
    }

    {
        Lamp::list<int> erase_test;
        for (int i = 0; i < 100; ++i) {
            erase_test.push_back(i);
        }

        for (int i = 0; i < 100; ++i) {
            if (*erase_test.at(i) != i) {
                std::cout << "mistmatch found at : " << i << std::endl;
            }
        }

        erase_test.erase(101);
        erase_test.erase(0);
        erase_test.erase(0, 90);

        PrintList(erase_test);
    }

    {
        Lamp::unordered_map<Lamp::String, int> map;
        map["Test1"] = 1;
        map["Test2"] = 2;
        map["Test3"] = 3;
        map["Test4"] = 4;
        map["Test5"] = 5;
        std::cout << std::endl;
        std::cout << "map[Test1] : " << map["Test1"] << std::endl;
        std::cout << "map[Test2] : " << map["Test2"] << std::endl;
        std::cout << "map[Test3] : " << map["Test3"] << std::endl;
        std::cout << "map[Test4] : " << map["Test4"] << std::endl;
        std::cout << "map[Test5] : " << map["Test5"] << std::endl;

        map.erase("Test6");
        map.erase("Test7");
        map.erase("Test7");
        map.erase("Test7");
        map.erase("Test7");

        map.erase("Test5");
        map.erase("Test5");
        map.erase("Test5");
        map.erase("Test5");
        map.erase("Test5");
        map.erase("Test3");
        map.erase("Test4");

        std::cout << "map[Test1] : " << map["Test1"] << std::endl;
        std::cout << "map[Test2] : " << map["Test2"] << std::endl;
        std::cout << "map[Test3] : " << map["Test3"] << std::endl;
        std::cout << "map[Test4] : " << map["Test4"] << std::endl;
        std::cout << "map[Test5] : " << map["Test5"] << std::endl;
    }

    {
        Lamp::unordered_map<uint32_t, uint32_t> uintmap;

        uintmap[1] = 2;
        uintmap[3] = 4;
        uintmap[5] = 6;
        uintmap[7] = 8;
        uintmap[9] = 10;

        std::cout << "map[1] : " << uintmap[1] << std::endl;
        std::cout << "map[2] : " << uintmap[2] << std::endl;
        std::cout << "map[3] : " << uintmap[3] << std::endl;
        std::cout << "map[4] : " << uintmap[4] << std::endl;
        std::cout << "map[5] : " << uintmap[5] << std::endl;
        std::cout << "map[6] : " << uintmap[6] << std::endl;
        std::cout << "map[7] : " << uintmap[7] << std::endl;
        std::cout << "map[8] : " << uintmap[8] << std::endl;
        std::cout << "map[9] : " << uintmap[9] << std::endl;
        std::cout << std::endl;
    }

    {
        Lamp::Vector<Lamp::String> test_v1;
        Lamp::Vector<Lamp::String> test_v2;

        Lamp::Vector<Lamp::String> test_v3;
        Lamp::Vector<Lamp::String> test_v4;

        Lamp::String testerv1 = "teester1";
        Lamp::String testerv2 = "teester2";

        test_v1.push_back("1");
        test_v1.push_back("2");
        test_v1.push_back("3");
        test_v1.push_back("4");
        test_v1.push_back("5");
        test_v1.push_back("6");

        test_v2.push_back("100");
        test_v2.push_back("101");
        test_v2.push_back("102");
        test_v2.push_back("103");
        test_v2.push_back("104");
        test_v2.push_back("105");

        test_v1 = test_v2;

        for (int i = 0; i < test_v1.size(); ++i) {
            std::cout << "v1 : " << test_v1[i].c_str() << std::endl;
        }

        test_v3.push_back(testerv1);

        test_v4.push_back(testerv2);
        test_v4.push_back(testerv2);

        test_v3 = test_v4;

        for (int i = 0; i < test_v3.size(); ++i) {
            std::cout << "v3 : " << test_v3[i].c_str() << std::endl;
        }


        test_v3.push_back("Inplace");
        test_v3.push_back("Inplace");
        test_v3.push_back("Inplace");
        test_v3.push_back("Inplace");
        test_v3.push_back("Inplace");

        test_v4.push_back(Lamp::String("Inplace2"));
        test_v4.push_back(Lamp::String("Inplace2"));
        test_v4.push_back(Lamp::String("Inplace2"));

        test_v4 = test_v3;

        for (int i = 0; i < test_v4.size(); ++i) {
            std::cout << "v4 : " << test_v4[i].c_str() << std::endl;
        }
    }

    {
        Lamp::Vec4f vector;
        vector.x = 0;
        vector.y = 0;
        vector.z = 0;
        vector.w = 1;

        {
            Lamp::Vec3f left, right;

            left = {3,0,2};
            right = {-1, 4, 2};

            auto result = left.Cross(right);
            std::cout << result.x << ", " << result.y << ", " << result.z << std::endl;
        }
        {
            Lamp::Mat4f left, right;
            left.c0 = {5,2,8,3};
            left.c1 = {7,3,10,3};
            left.c2 = {9,3,2,4};
            left.c3 = {10,8,3,8};

            right.c0 = {3, 12, 9, 3};
            right.c1 = {10, 1, 10, 12};
            right.c2 = {12, 4, 12, 4};
            right.c3 = {18, 9, 2, 10};

            Lamp::Mat4f result = left * right;

            std::cout << "[ " << result.c0.x << ", " << result.c1.x << ", " << result.c2.x << ", " << result.c3.x << "]" << std::endl;
            std::cout << "[ " << result.c0.y << ", " << result.c1.y << ", " << result.c2.y << ", " << result.c3.y << "]" << std::endl;
            std::cout << "[ " << result.c0.z << ", " << result.c1.z << ", " << result.c2.z << ", " << result.c3.z << "]" << std::endl;
            std::cout << "[ " << result.c0.w << ", " << result.c1.w << ", " << result.c2.w << ", " << result.c3.w << "]" << std::endl;
        }

        {
            Lamp::Vector<int> range_based_for;
            range_based_for.push_back(1);
            range_based_for.push_back(2);
            range_based_for.push_back(3);
            range_based_for.push_back(4);
            range_based_for.push_back(5);
            range_based_for.push_back(6);
            range_based_for.push_back(7);
            for (int& i : range_based_for) {
                std::cout << "Range based for :" << i << std::endl;
            }

            for (const int& i : range_based_for) {
                std::cout << "const Range based for :" << i << std::endl;
            }

            for (const int i : range_based_for) {
                std::cout << "another range based for :" << i << std::endl;
            }
        }
    }

    {
        using namespace Lamp;
        unordered_map<uint32_t, float> map;
        map[1] = 2;
        map[2] = 3;
        map[3] = 4;
        map[4] = 5;
        map[5] = 6;

        // Sicne it is unordered map, the order must be random.
        for (const auto& p : map) {
            std::cout <<  p.first << " : " << p.second << std::endl;
        }
    }

    std::cout << std::endl;
    return 0;

}
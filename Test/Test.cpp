#include <iostream>
#include "../lampAllocator.h++"

#include "../lampString.h++"
#include "../lampVector.h++"
#include "../lampCRC32.h++"
#include "../lampMurmur3.h++"
#include "../lampList.h++"
#include "../lampUnordered_map.h++"
#include "../lampMath.h++"
#include "../lampDeque.h++"
#include "../lampRandom.h++"
#include "../lampStack.h++"
#include "../lampQueue.h++"
#include "../lampBinarySearchTree.h++"
#include "../lampAlgorithm.h++"

#include <vector>
#include <deque>
#include <chrono>
#include <random>


class TimeStamp {
    static std::chrono::steady_clock::time_point start_;
    static std::chrono::steady_clock::time_point end_;

public:
    static void Start() {
        start_ = std::chrono::steady_clock::now();
    }

    static void End() {
        end_ = std::chrono::steady_clock::now();
    }

    static std::chrono::duration<double> Duration() {
        return end_ - start_;
    }
    static TimeStamp& instance;
};

// Huge Pod Struct for testing..
struct HugeStruct {
    double d;
    float f;
    int i;
    char c;

    double darr[16];

    HugeStruct() = default;

    HugeStruct(const int& _i) {
        // do the random thing..
        d = _i;
        f = _i;
        i = _i;
        c = 0;
        memcpy(&c, &_i, 1);


        for (int j = 0; j < 16; ++j) {
            if (j % 3 == 0)
                darr[j] = d;
            else if (j % 3 == 0)
                darr[j] = f;
            else if (j % 3 == 0)
                darr[j] = i;
        }
    }

    HugeStruct(int&& _i) {
        // do the same thing..
        d = std::move(_i);
        f = _i;
        i = _i;
        c = 0;
        memcpy(&c, &_i, 1);


        for (int j = 0; j < 16; ++j) {
            if (j % 3 == 0)
                darr[j] = d;
            else if (j % 3 == 0)
                darr[j] = f;
            else if (j % 3 == 0)
                darr[j] = i;
        }
    }


    HugeStruct(HugeStruct&& _rhs)
        : d(std::move(_rhs.d)), f(std::move(_rhs.f)), i(std::move(_rhs.i)), c(std::move(_rhs.c)) {
        memcpy(darr, _rhs.darr, sizeof(double) * 16);
    }

    HugeStruct(const HugeStruct& _rhs)
        : d(_rhs.d), f(_rhs.f), i(_rhs.i), c(_rhs.c) {
        memcpy(darr, _rhs.darr, sizeof(double) * 16);
    }

    HugeStruct& operator=(HugeStruct&& _rhs) {
        if (this != &_rhs) {
            d = std::move(_rhs.d);
            f = std::move(_rhs.f);
            i = std::move(_rhs.i);
            c = std::move(_rhs.c);
            memcpy(darr, _rhs.darr, sizeof(double) * 16);
        }

        return *this;
    }

    HugeStruct& operator=(const HugeStruct& _rhs) {
        if (this != &_rhs) {
            d = (_rhs.d);
            f = (_rhs.f);
            i = (_rhs.i);
            c = (_rhs.c);
            memcpy(darr, _rhs.darr, sizeof(double) * 16);
        }

        return *this;
    }

    bool operator!=(const HugeStruct& _rhs) {
        bool result = true;

        result = result && (_rhs.d != d);
        result = result && (_rhs.i != i);
        result = result && (_rhs.c != c);
        result = result && (_rhs.f != f);

        for (int i = 0; i < 16; ++i) {
            result = result && (_rhs.darr[i] != darr[i]);
        }

        return result;
    }
} __attribute__((packed));

std::chrono::steady_clock::time_point TimeStamp::start_;
std::chrono::steady_clock::time_point TimeStamp::end_;

TimeStamp gTimeStamp;
TimeStamp& TimeStamp::instance = gTimeStamp;

template<typename T>
void PrintList(Lamp::list<T> & _list) {
    while (!_list.empty()) {
        std::cout << _list.front() << ", ";
        _list.pop_front();
    }
}

template<typename T>
void PrintVector(Lamp::Vector<T>& _vector) {
    for (auto& i : _vector) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
}

template<typename T, size_t segmentSize>
void PrintDeque(Lamp::deque<T, segmentSize> & _array) {
    for (size_t i = 0; i < _array.size(); ++i) {
        std::cout << _array[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "front : " << _array.front() << ", end : " << _array.back() << std::endl;
    std::cout << "total count : " << _array.size() << std::endl;
    std::cout << std::endl;
}

template<size_t _initial_cap>
bool RandomDequeTest(size_t _count, uint32_t _seed, bool _is_verbose) {
    enum class Action {
        push_back = 0,
        push_front,
        pop_back,
        pop_front
    };
    std::stringstream output;
    const unsigned count = _count; //failed 20(operator[]), 100(value test failed.)
    Lamp::random_device<uint32_t> randdevice(static_cast<uint32_t>(_seed));

    Lamp::deque<int, _initial_cap> deque;

    Lamp::Vector<Action> actions;
    Lamp::Vector<int> values;

    actions.reserve(count);
    values.reserve(count);

    unsigned curr_count = 0;
    for (unsigned i = 0; i < count; ++i) {
        Action a = static_cast<Action>( randdevice.RandRange(4));

        if ((a == Action::pop_back || a == Action::pop_front) && curr_count == 0) {
            continue;
        }

        actions[i] = a;
        values[i] = randdevice.RandIntBetween(-1000, 1000);

        if (a == Action::push_back || a == Action::push_front)
            ++curr_count;
        else
            --curr_count;
    }

    Lamp::Vector<int> expected;
    expected.reserve(count);
    unsigned start = 0;
    unsigned expected_c = 0;

    for (unsigned i = 0; i < count; ++i) {
        const Action& a = actions[i];
        const int& v = values[i];
        switch (a) {
            case Action::push_back:
                expected[(start + expected_c) % count] = v;
                ++expected_c;
                //std::cout << "deq.push_back(" << v  << ");" << std::endl;
            break;
            case Action::pop_back:
                --expected_c;
                //std::cout << "deq.pop_back();" << std::endl;
            break;
            case Action::push_front:
                start = (start + count -1) % count;
                expected[start] = v;
                ++expected_c;
                //std::cout << "deq.push_front(" << v  << ");" << std::endl;
            break;
            case Action::pop_front:
                --expected_c;
                start = (start + 1) % count;
                //std::cout << "deq.pop_front();" << std::endl;
            break;
        }
    }



    output << "Expected count : " <<  expected_c << std::endl;
    for (unsigned i = 0; i < count; ++i) {
        const Action& a = actions[i];
        const int& v = values[i];
        switch (a) {
            case Action::push_back:
                //std::cout << "deq.push_back(" << v  << ");" << std::endl;

                deque.push_back(v);

            break;
            case Action::pop_back:
                //std::cout << "deq.pop_back();" << std::endl;

                deque.pop_back();
            break;
            case Action::push_front:
                //std::cout << "deq.push_front(" << v  << ");" << std::endl;

                deque.push_front(v);
            break;
            case Action::pop_front:
                //std::cout << "deq.pop_front();" << std::endl;

                deque.pop_front();
            break;
        }
    }


    output << "Actual count : " << deque.size() << std::endl;
    bool is_fail = false;
    for (unsigned i = 0; i < deque.size(); ++i) {
        if (deque[i] != expected[(start + i) % count]) {
            is_fail = true;
            output << "Value difference in index " << i << " with value " <<
                deque[i]
            << " : (expected)" << expected[(start + i) % count] << std::endl;
        }
    }

    // Range based for test
    {
        size_t j = 0;
        for (auto& i : deque) {

            if (i != expected[(start + j) % count]) {
                is_fail = true;
                output << "Value difference during range based for loop " << " with value " <<
                    i
                << " : (expected)" << expected[(start + j) % count] << std::endl;
            }

            ++j;
        }
    }


    if (is_fail) {
        for (unsigned i = 0; i < expected_c; ++i) {
            output << expected[(start + i) % count] << ", ";
            if (i % 20 == 19)
                output << std::endl;
        }
        output << std::endl;

        if (_is_verbose) {
            std::cout << output.str() << std::endl;
            PrintDeque(deque);
        }
    }
    else {
        //std::cout << "Passed random deque test." << std::endl;
    }



    return !is_fail;
}

template<size_t _initial_cap>
bool dequeComparison(size_t _count, uint32_t _seed, bool _is_verbose) {
    enum class Action {
        push_back = 0,
        push_front,
        pop_back,
        pop_front
    };
    std::stringstream output;
    const unsigned count = _count; //failed 20(operator[]), 100(value test failed.)
    Lamp::random_device<uint32_t> randdevice(static_cast<uint32_t>(_seed));

    Lamp::deque<int, _initial_cap> deque;
    std::deque<int> std;

    Lamp::Vector<Action> actions;
    Lamp::Vector<int> values;

    actions.reserve(count);
    values.reserve(count);

    unsigned curr_count = 0;
    for (unsigned i = 0; i < count; ++i) {
        Action a = static_cast<Action>( randdevice.RandRange(4));

        if ((a == Action::pop_back || a == Action::pop_front) && curr_count == 0) {
            continue;
        }

        actions[i] = a;
        values[i] = randdevice.RandIntBetween(-1000, 1000);

        if (a == Action::push_back || a == Action::push_front)
            ++curr_count;
        else
            --curr_count;
    }

    TimeStamp::Start();
    for (unsigned i = 0; i < count; ++i) {
        const Action& a = actions[i];
        const int& v = values[i];
        switch (a) {
            case Action::push_back:
                deque.push_back(v);
            break;
            case Action::pop_back:
                deque.pop_back();
            break;
            case Action::push_front:
                deque.push_front(v);
            break;
            case Action::pop_front:
                deque.pop_front();
            break;
        }
    }
    TimeStamp::End();
    auto lamp_duration = TimeStamp::Duration();

    TimeStamp::Start();
    for (unsigned i = 0; i < count; ++i) {
        const Action& a = actions[i];
        const int& v = values[i];
        switch (a) {
            case Action::push_back:
                std.push_back(v);
            break;
            case Action::pop_back:
                std.pop_back();
            break;
            case Action::push_front:
                std.push_front(v);
            break;
            case Action::pop_front:
                std.pop_front();
            break;
        }
    }
    TimeStamp::End();
    auto std_duration = TimeStamp::Duration();

    std::cout << "For insert, " << std::endl;
    std::cout << "Lamp : " << lamp_duration << std::endl;
    std::cout << "std  : " << std_duration << std::endl;

    // Random access test
    volatile int s = 0;
    TimeStamp::Start();
    for (size_t i = 0; i < deque.size(); ++i) {
        s += deque[i];
    }
    TimeStamp::End();
    lamp_duration = TimeStamp::Duration();

    std::cout << s;
    s = 0;

    TimeStamp::Start();
    for (size_t i = 0; i < std.size(); ++i) {
        s += std[i];
    }
    TimeStamp::End();
    std_duration = TimeStamp::Duration();

    std::cout << s;

    std::cout << std::endl;
    std::cout << "For random access, " << std::endl;
    std::cout << "Lamp : " << lamp_duration << std::endl;
    std::cout << "std  : " << std_duration << std::endl;


    return true;
}

bool RandomVectorTest(size_t _count, uint32_t _seed, bool _is_verbose) {
    int* arr = new int[_count];
    Lamp::Vector<int> lamp;
    std::stringstream output;
    Lamp::random_device<uint32_t> randdevice(static_cast<uint32_t>(_seed));

    for (size_t i = 0; i < _count; ++i) {
        arr[i] = randdevice.RandIntBetween(-4000, 4000);
        lamp.push_back(arr[i]);
    }

    // In my implementation, there is no input functino beside push_back.
    for (size_t i = 0; i < _count; ++i) {
        if (arr[i] != lamp[i]) {
            delete[] arr;
            return false;
        }
    }

    delete[] arr;
    return true;
}

template<typename T>
bool RandomListTest(size_t _count, uint32_t _seed, bool _is_verbose) {
    enum class Action {
        push_back = 0,
        push_front,
        pop_back,
        pop_front
    };

    Lamp::list<T> lamp;
    std::stringstream output;
    Lamp::random_device<uint32_t> randdevice(static_cast<uint32_t>(_seed));


    Lamp::Vector<Action> actions;
    Lamp::Vector<int> values;

    actions.reserve(_count);
    values.reserve(_count);

    size_t curr_count = 0;
    for (size_t i = 0; i < _count; ++i) {
        Action a = static_cast<Action>( randdevice.RandRange(4));

        if ((a ==Action:: pop_back || a == Action::pop_front) && curr_count == 0) {
            continue;
        }

        actions[i] = a;
        values[i] = randdevice.RandIntBetween(-1000, 1000);

        if (a == Action::push_back || a == Action::push_front)
            ++curr_count;
        else
            --curr_count;
    }

    Lamp::Vector<int> expected;
    expected.reserve(_count);
    size_t start = 0;
    size_t expected_c = 0;

    for (size_t i = 0; i < _count; ++i) {
        const Action& a = actions[i];
        const int& v = values[i];
        switch (a) {
            case Action::push_back:
                expected[(start + expected_c) % _count] = v;
                ++expected_c;
            break;
            case Action::pop_back:
                --expected_c;
            break;
            case Action::push_front:
                start = (start + _count -1) % _count;
                expected[start] = v;
                ++expected_c;
            break;
            case Action::pop_front:
                --expected_c;
                start = (start + 1) % _count;
            break;
        }
    }

    for (size_t i = 0; i < _count; ++i) {
        const Action& a = actions[i];
        const int& v = values[i];
        switch (a) {
            case Action::push_back:
                lamp.push_back(v);
            break;
            case Action::pop_back:
                lamp.pop_back();
            break;
            case Action::push_front:
                lamp.push_front(v);
            break;
            case Action::pop_front:
                lamp.pop_front();
            break;
        }
    }

    bool is_fail = false;
    if (lamp.size() != expected_c) {
        is_fail = true;
        std::cout << "RandomListTest: count not matching." << std::endl;
        return !is_fail;
    }

    if (lamp.empty())
        return true;

    for (size_t i = 0; i < expected_c; ++i) {
        if (lamp.front() != expected[(start + i) % _count]) {
            is_fail = true;
            output << "Value difference in index " << i << " with value " <<
                lamp.front()
            << " : (expected)" << expected[(start + i) % _count] << std::endl;
        }

        lamp.pop_front();
    }

    if (_is_verbose) {
        std::cout << std::endl << output.str() << std::endl;
    }

    return true;
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

    if (false)
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
        PrintList(list); std::cout << std::endl;
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

    if (false)
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

    if (false)
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

    if (false)
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

    if (false)
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

    if (false)
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

    if (false)
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

    if (true)
    {
        Lamp::deque<int, 4> segment_tree;
        segment_tree.push_back(-1);
        segment_tree.push_back(-2);
        segment_tree.push_back(-3);
        segment_tree.push_back(-4);
        //PrintDeque(segment_tree); // -1, -2, -3, -4

        segment_tree.pop_front();
        PrintDeque(segment_tree); // -2, -3, -4

        segment_tree.pop_back();
        PrintDeque(segment_tree); // -2, -3

        segment_tree.push_front(-1);
        PrintDeque(segment_tree); // -1, -2, -3

        segment_tree.push_back(-4);
        PrintDeque(segment_tree); // -1, -2, -3, -4

        segment_tree.push_back(-5);
        PrintDeque(segment_tree); // -1, -2, -3, -4, -5
    }

    if (false)
    {
        Lamp::random_device<uint32_t> randdevice;
        std::cout << randdevice.XORShift32StarStep() << std::endl;
        std::cout << randdevice.LCGStep() << std::endl;
        std::cout << randdevice.Rand() << std::endl;
        std::cout << randdevice.RandRange(100) << std::endl;
        std::cout << randdevice.RandBetween(20, 100) << std::endl;
        std::cout << randdevice.RandBool() << std::endl;
        std::cout << randdevice.RandFloat(2048) << std::endl;
        std::cout << randdevice.RandFloat(4096) << std::endl;
        std::cout << randdevice.RandFloat(8192) << std::endl;
        std::cout << randdevice.RandIntBetween(-100, 100) << std::endl;
        std::cout << randdevice.RandNormalizedFloat(8192) << std::endl;

        Lamp::Vector<int> v;
        for (int i = 0; i < 100; ++i) {
            v.push_back(i);
        }

        randdevice.Shuffle(v.data(), v.size());
        for (int i = 0; i < v.size(); ++i) {
            std::cout << v[i] << ", ";
            if (i % 20 == 0)
                std::cout << std::endl;
        }
        std::cout << std::endl;
    }


    if (true) {
        Lamp::deque<int, 3> deque;

        for (int i = 0; i < 16; ++i) {
            deque.push_back(i);
        }

        PrintDeque(deque);
    }


    if (false) {
        std::cout << "========= capacity 3 ==========" << std::endl;
        TimeStamp::Start();
        for (size_t i = 0; i < 50000; ++i) {
            if (!RandomDequeTest<3>(i, 12345, false))
                std::cout << "Failed test with i : " << i << std::endl;
            //else
            //    std::cout << "Passed test with i : " << i << std::endl;
        }
        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;

        std::cout << "========= capacity 1 ==========" << std::endl;
        TimeStamp::Start();
        for (size_t i = 0; i < 50000; ++i) {
            if (!RandomDequeTest<1>(i, 23456, false))
                std::cout << "Failed test with i : " << i << std::endl;
            //else
            //    std::cout << "Passed test with i : " << i << std::endl;
        }
        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;


        std::cout << "========= capacity 2 ==========" << std::endl;
        TimeStamp::Start();
        for (size_t i = 0; i < 50000; ++i) {
            if (!RandomDequeTest<2>(i, 34567, false))
                std::cout << "Failed test with i : " << i << std::endl;
            //else
            //    std::cout << "Passed test with i : " << i << std::endl;
        }
        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;


        std::cout << "========= capacity 77 ==========" << std::endl;
        TimeStamp::Start();
        for (size_t i = 0; i < 50000; ++i) {
            if (!RandomDequeTest<77>(i, 56789, false))
                std::cout << "Failed test with i : " << i << std::endl;
            //else
            //    std::cout << "Passed test with i : " << i << std::endl;
        }
        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;
    }


    if (false) {
        dequeComparison<4>(200000001, 99999, false);
    }

    if (false) {
        constexpr uint32_t seed = 12345;
        Lamp::random_device<uint32_t> randdevice(static_cast<uint32_t>(seed));
        std::cout << "lamp::stack stress test" << std::endl;

        {
            constexpr size_t count = 500000;
            Lamp::stack<int, 3> stack;
            int* arr = new int[count];

            for (size_t i = 0; i < count; ++i) {
                int rand = randdevice.RandIntBetween(-4000, 4000);
                stack.push(rand);
                arr[(count -1) - i] = rand;
            }

            if (stack.size() != count) {
                std::cout << "Incorrect size." << std::endl;
            }

            for (size_t i = 0; i < count; ++i) {

                if (stack.top() != arr[i]) {
                    std::cout << "Failed stack test." << i << std::endl;
                }

                stack.pop();
            }

            delete[] arr;
        }
    }

    if (false) {
        std::cout << "Performance measure" << std::endl;
        std::cout << "vector" << std::endl;
        TimeStamp::Start();
        Lamp::Vector<int> vector;
        for (size_t times = 0; times < 5; ++times) {
            for (size_t i = 0; i < 500000000; ++i) {
                vector.push_back(i);
            }
            vector.clear();
        }
        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;
        // with normal new :
        // 9.74411s
        // 9.9182s
        // 9.36578s
        // 9.56895s
        // 9.35392s
        // 9.46613s
        // 9.37823s
        // 10.5824s
        // 9.39467s
        // 9.42515s
    }

    // unordered_map stress test
    if (false) {
        std::cout << "unordered_map stress test" << std::endl;
        TimeStamp::Start();
        Lamp::unordered_map<int, void*> map;
        for (size_t times = 0; times < 5; ++times) {
            for (size_t i = 0; i < 400000; ++i) {
                map[i] = &map[i];
            }
        }
        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;
    }

    if (false) {
        TimeStamp::Start();
        for (size_t i = 0; i < 50000; ++i) {
            if (!RandomVectorTest(i, 12345, false)) {
                std::cout << "Random vector test failed : " << i << std::endl;
            }
        }
        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;
    }

    if (false) {
        TimeStamp::Start();
        for (size_t i = 0; i < 50000; ++i) {
            if (!RandomListTest<int>(i, 12345, false)) {
                std::cout << "Random list test failed : " << i << std::endl;
            }
        }
        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;
    }

    // List erase test.
    if (false) {
        Lamp::random_device<uint32_t> randdevice;
        Lamp::list<int> list;

        TimeStamp::Start();
        for (size_t i = 0; i < 25; ++i) {
            list.push_back(i);
        }
        std::cout << list.size() << std::endl;

        for (size_t i = 0; i < 25; ++i) {
            if (i % 2 == 0)
                list.erase(i);
        }
        std::cout << list.size() << std::endl;

        while (!list.empty()) {
            std::cout << list.front() << ",";
            list.pop_front();
        }
        std::cout << std::endl << list.size() << std::endl;


        TimeStamp::End();
        std::cout << TimeStamp::Duration() << std::endl;
    }

    if (false) {

        std::cout << "lamp::queue" << std::endl;
        constexpr size_t count = 100;

        Lamp::random_device<uint32_t> randdevice(12345);
        Lamp::queue<int> lamp;
        int* arr = new int[count];

        TimeStamp::Start();
        TimeStamp::End();
        for (size_t i = 0; i < count; ++i) {
            int rand = randdevice.RandIntBetween(-4000, 4000);

            arr[i] = rand;
            lamp.push(rand);
        }

        if (lamp.size() != count) {
            std::cout << "Failed queue size test." << std::endl;
        }

        for (size_t i = 0; i < count; ++i) {
            if (arr[i] != lamp.top()) {
                std::cout << "Failed queue test with index : " << i << std::endl;
            }

            lamp.pop();
        }

        std::cout << TimeStamp::Duration() << std::endl;
        delete[] arr;
    }

    //Binary search tree base.
    if (false) {
        std::cout << "Binary search tree implementation test" << std::endl;

        for (int i = 0; i <= 10; ++i) {
            Lamp::BinarySearchTree<int> bst;

            for (int j = 10; j >= 0; --j) {
                bst.insert(j);
            }

            bst.erase(i);

            std::cout << "min : " << bst.min() << ", ";
            std::cout << "max : " << bst.max() << ", ";
            std::cout << "size : " << bst.size() << std::endl;
        }
    }

    if (false) {
        std::cout << "Binary search tree number test" << std::endl;
        //[5, 5, 3, 7, 3, 1, 9, 5, 7, 0, 10]

        Lamp::BinarySearchTree<int> bst;
        bst.insert(5);
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);
        bst.insert(3);
        bst.insert(1);
        bst.insert(9);
        bst.insert(5);
        bst.insert(7);
        bst.insert(0);
        bst.insert(10);

        std::cout << "min : " << bst.min() << ", ";
        std::cout << "max : " << bst.max() << ", ";
        std::cout << "size : " << bst.size() << std::endl;

        auto exist_test = [&](int i) {
            if (!bst.exist(i))
                std::cout << i << " not exist." << std::endl;
        };

        exist_test(5);
        exist_test(5);
        exist_test(3);
        exist_test(7);
        exist_test(3);
        exist_test(1);
        exist_test(9);
        exist_test(0);
        exist_test(10);
        exist_test(7);
        exist_test(5);

        exist_test(11); // should not exist
        exist_test(12); // should not exist

        bst.erase(5);
        exist_test(5);
        bst.erase(5);
        exist_test(5);
        bst.erase(5);
        exist_test(5); // should not exist
    }

    if (false) {
        std::cout << "Binary tree performance measure" << std::endl;

        Lamp::random_device<uint32_t> randdevice(12345);
        int min = 5000;
        int max = -5000;

        constexpr size_t count = 800000;
        constexpr int loop_count = 20;

        std::chrono::duration<double> s;

        for (int loop = 0; loop < loop_count; ++loop) {
            TimeStamp::Start();
            Lamp::BinarySearchTree<int> bst;
            for (size_t i = 0; i < count; ++i) {
                int rand = randdevice.RandIntBetween(-4000, 4000);
                min = std::min(rand, min);
                max = std::max(rand, max);
                bst.insert(rand);
            }

            if (bst.min() != min) {
                std::cout << "min value not matching." << std::endl;
            }

            if (bst.max() != max) {
                std::cout << "max value not matching." << std::endl;
            }

            bst.insert(0);
            bst.exist(0);

            TimeStamp::End();
            auto duration = TimeStamp::Duration();
            std::cout << duration << std::endl;
            s += duration;
        }

        std::cout << s / loop_count << " in average." << std::endl;
        // recursive version : 5.75988s in average.
        //5.68381s in average.
    }

    if (true) {
        std::cout << "deque emplace_back test" << std::endl;
        {
            Lamp::deque<HugeStruct> d;

            d.push_front(7);
            d.push_back(8);
            d.push_front(6);
            d.push_back(9);
            d.push_front(5);
            d.push_back(10);
            d.push_front(4);
            d.push_back(11);
            d.push_front(3);
            d.push_back(12);
            d.push_front(2);
            d.push_back(13);
            d.push_front(1);
            d.push_back(14);
            d.push_front(0);
            d.push_back(15);

            for (int i = 0; i < 16; ++i) {
                std::cout << d[i].i << ", ";
            }
            std::cout << std::endl;

            for (auto& i : d) {
                std::cout << i.i << ", ";
            }
            std::cout << std::endl;
        }


        {
            Lamp::deque<HugeStruct> d;

            d.emplace_front(7);
            d.emplace_back(8);
            d.emplace_front(6);
            d.emplace_back(9);
            d.emplace_front(5);
            d.emplace_back(10);
            d.emplace_front(4);
            d.emplace_back(11);
            d.emplace_front(3);
            d.emplace_back(12);
            d.emplace_front(2);
            d.emplace_back(13);
            d.emplace_front(1);
            d.emplace_back(14);
            d.emplace_front(0);
            d.emplace_back(15);

            for (auto& i : d) {
                std::cout << i.i << ", ";
            }
            std::cout << std::endl;
        }
    }

    // Range based for performance comparison test
    if (true) {
        Lamp::random_device<uint32_t> randdevice(12345);
        constexpr size_t vcount = 10000000;

        constexpr bool vector_tests = false;
        constexpr bool std_deque_tests = false;
        constexpr bool lamp_deque_tests = false;


        std::cout << "Performance comparison" << std::endl;
        if (vector_tests)
        {
            std::vector<HugeStruct> std;
            Lamp::Vector<HugeStruct> lamp;

            TimeStamp::Start();
            for (size_t i = 0; i < vcount; ++i) {
                std.push_back(randdevice.RandIntBetween(-4000, 4000));
            }
            TimeStamp::End();
            std::cout << "std::vector::push_back " << TimeStamp::Duration() << std::endl;

            TimeStamp::Start();
            for (size_t i = 0; i < vcount; ++i) {
                lamp.push_back(randdevice.RandIntBetween(-4000, 4000));
            }
            TimeStamp::End();
            std::cout << "Lamp::Vector::push_back " << TimeStamp::Duration() << std::endl;


            TimeStamp::Start();
            for (size_t i = 0; i < vcount; ++i) {
                std.emplace_back(randdevice.RandIntBetween(-4000, 4000));
            }
            TimeStamp::End();
            std::cout << "std::vector::emplace_back " << TimeStamp::Duration() << std::endl;

            TimeStamp::Start();
            for (size_t i = 0; i < vcount; ++i) {
                lamp.emplace_back(randdevice.RandIntBetween(-4000, 4000));
            }
            TimeStamp::End();
            std::cout << "Lamp::Vector::emplace_back " << TimeStamp::Duration() << std::endl;




            volatile int s = 0;
            TimeStamp::Start();
            for (auto& i : std) {
                s += i.i;
                s += i.c;
                s += static_cast<int>(i.d);
                s += static_cast<int>(i.darr[s % 16]);
            }
            TimeStamp::End();
            std::cout << "std::vector::for loop& " << TimeStamp::Duration() << std::endl;


            TimeStamp::Start();
            for (auto& i : lamp) {
                s += i.i;
                s += i.c;
                s += static_cast<int>(i.d);
                s += static_cast<int>(i.darr[s % 16]);
            }
            TimeStamp::End();
            std::cout << "Lamp::Vector::for loop& " << TimeStamp::Duration() << std::endl;

            TimeStamp::Start();
            for (auto i : std) {
                s += i.i;
                s += i.c;
                s += static_cast<int>(i.d);
                s += static_cast<int>(i.darr[s % 16]);
            }
            TimeStamp::End();
            std::cout << "std::vector::for loop copy" << TimeStamp::Duration() << std::endl;


            TimeStamp::Start();
            for (auto i : lamp) {
                s += i.i;
                s += i.c;
                s += static_cast<int>(i.d);
                s += static_cast<int>(i.darr[s % 16]);
            }
            TimeStamp::End();
            std::cout << "Lamp::Vector::for loop copy" << TimeStamp::Duration() << std::endl;
        }

        constexpr size_t dcount = 40000000;
        {
            volatile size_t si = 0;
            volatile size_t sj = 0;

            if (std_deque_tests)
            {
                std::deque<HugeStruct> std;

                TimeStamp::Start();
                Lamp::random_device<uint32_t> randdevice(23456);

                for (size_t i = 0; i < dcount; ++i) {
                    std.push_back(randdevice.RandIntBetween(-4000, 4000));
                    std.push_front(randdevice.RandIntBetween(-4000, 4000));
                }
                TimeStamp::End();
                std::cout << "std::deque::push_back and front " << TimeStamp::Duration() << std::endl;


                TimeStamp::Start();
                for (size_t i = 0; i < dcount; ++i) {
                    std.emplace_back(randdevice.RandIntBetween(-4000, 4000));
                    std.emplace_front(randdevice.RandIntBetween(-4000, 4000));
                }
                TimeStamp::End();
                std::cout << "std::deque::emplace_back and front " << TimeStamp::Duration() << std::endl;


                TimeStamp::Start();
                for (auto& i : std) {
                    si += i.i;
                    si += i.c;
                    //si += static_cast<int>(i.d);
                    //si += static_cast<int>(i.darr[si % 16]);
                }
                TimeStamp::End();
                std::cout << "std::deque::for loop " << TimeStamp::Duration() << std::endl;

                TimeStamp::Start();
            }

            if (std_deque_tests) {
                TimeStamp::End();
                std::cout << "std::deque::~deque() " << TimeStamp::Duration() << std::endl;
            }


            if (lamp_deque_tests)
            {
                Lamp::deque<HugeStruct> lamp;

                TimeStamp::Start();
                Lamp::random_device<uint32_t> randdevice(23456);

                for (size_t i = 0; i < dcount; ++i) {
                    lamp.push_back(randdevice.RandIntBetween(-4000, 4000));
                    lamp.push_front(randdevice.RandIntBetween(-4000, 4000));
                }
                TimeStamp::End();
                std::cout << "Lamp::deque::push_back and front " << TimeStamp::Duration() << std::endl;


                TimeStamp::Start();

                for (size_t i = 0; i < dcount; ++i) {
                    lamp.emplace_back(randdevice.RandIntBetween(-4000, 4000));
                    lamp.emplace_front(randdevice.RandIntBetween(-4000, 4000));
                }
                TimeStamp::End();
                std::cout << "Lamp::deque::emplace_back and front " << TimeStamp::Duration() << std::endl;


                TimeStamp::Start();
                for (auto& i : lamp) {
                    sj += i.i;
                    sj += i.c;
                    //sj += static_cast<int>(i.d);
                    //sj += static_cast<int>(i.darr[sj % 16]);
                }
                TimeStamp::End();
                std::cout << "Lamp::deque::for loop " << TimeStamp::Duration() << std::endl;

                TimeStamp::Start();
            }
            if (lamp_deque_tests) {
                TimeStamp::End();
                std::cout << "Lamp::deque::~deque() " << TimeStamp::Duration() << std::endl;
            }


            if (lamp_deque_tests && std_deque_tests && si != sj) {
                std::cout << "Lamp::deque and std::deque failed sum check." << std::endl;
            }
        }
    }

    // Algorithm : sort test
    if (false) {
        std::cout << "Bubble sort implementation test" << std::endl;
        Lamp::Vector<int> v;
        Lamp::deque<int> d;
        Lamp::list<int> l;
        Lamp::unordered_map<int, int> u;

        Lamp::random_device<uint32_t> rand(34567);

        constexpr size_t test_count = 1000;

        for (size_t i = 0; i < test_count; ++i) {
            v.push_back(rand.RandIntBetween(-5000, 5000));
            d.push_back(rand.RandIntBetween(-5000, 5000));
            l.push_back(rand.RandIntBetween(-5000, 5000));
            u[rand.RandIntBetween(-5000, 5000)] = rand.RandIntBetween(-5000, 5000);
        }
        Lamp::sort<Lamp::SortStrat::BubbleAscending>(v.begin(), v.end());
        for (size_t i = 1; i < test_count; ++i) {
            if (v[i-1] > v[i]) {
                std::cout << "Lamp::Vector not sorted properly." << std::endl;
                PrintVector(v);
                break;
            }
        }

        Lamp::sort<Lamp::SortStrat::BubbleAscending>(d.begin(), d.end());
        for (size_t i = 1; i < test_count; ++i) {
            if (d[i-1] > d[i]) {
                std::cout << "Lamp::deque not sorted properly." << std::endl;
                PrintDeque(d);
                break;
            }
        }

        Lamp::sort<Lamp::SortStrat::BubbleAscending>(l.begin(), l.end());
        {
            int prev = -5001;
            for (const auto& i : l) {
                if (prev > i) {
                    std::cout << "Lamp::unordered_map not sorted properly." << std::endl;
                }
                prev = i;
            }
        }

        Lamp::sort<Lamp::SortStrat::BubbleAscending>(u.begin(), u.end());
        {
            Lamp::pair<int, int> prev(-5001, 1);
            for (const auto& p : u) {
                if (prev.first > p.first) {
                    std::cout << "Lamp::unordered_map not sorted properly." << std::endl;
                }
                prev = p;
            }
        }
    }

    // Quick sort implementation test
    if (true) {
        std::cout << "Quick sort implementation test" << std::endl;
        Lamp::Vector<int> v;
        Lamp::random_device<uint32_t> rand(34567);

        constexpr size_t test_count = 1000;

        for (size_t i = 0; i < test_count; ++i) {
            v.push_back(rand.RandIntBetween(-5000, 5000));
        }

        Lamp::sort<Lamp::SortStrat::QuickAscending>(v.begin(), v.end());
        for (size_t i = 1; i < test_count; ++i) {
            if (v[i-1] > v[i]) {
                std::cout << "Lamp::Vector not sorted properly." << std::endl;
                PrintVector(v);
                break;
            }
        }


        Lamp::sort<Lamp::SortStrat::QuickDescending>(v.begin(), v.end());
        for (size_t i = 1; i < test_count; ++i) {
            if (v[i-1] < v[i]) {
                std::cout << "Lamp::Vector not sorted properly." << std::endl;
                PrintVector(v);
                break;
            }
        }
    }

    // Sort comparison
    // Quick should be faster, but it can be slower due to poor implementation.
    if (false) {
        constexpr size_t vcount = 40000000;
        std::cout << "Sort comparison" << std::endl;

        std::cout << "Bubble sort : SKIP"; // Skipping bubble sort, it is too slow to compare..
        /*
        {
            Lamp::Vector<int> v;
            Lamp::random_device<uint32_t> rand(34567);

            for (size_t i = 0; i < vcount; ++i) {
                v.push_back(rand.RandIntBetween(-5000, 5000));
            }
            TimeStamp::Start();
            Lamp::sort<Lamp::SortBubbleAscending>(v.begin(), v.end());
            TimeStamp::End();
            auto duration = TimeStamp::Duration();
            std::cout << duration << std::endl;
        }
        */
        std::cout << std::endl;;

        std::cout << "Quick sort : ";
        {
            Lamp::Vector<int> v;
            Lamp::random_device<uint32_t> rand(34567);

            for (size_t i = 0; i < vcount; ++i) {
                v.push_back(rand.RandIntBetween(-5000, 5000));
            }
            TimeStamp::Start();
            Lamp::sort<Lamp::SortStrat::QuickAscending>(v.begin(), v.end());
            TimeStamp::End();
            auto duration = TimeStamp::Duration();
            std::cout << duration << std::endl;
        }

        std::cout << "Heap sort : ";
        {
            Lamp::Vector<int> v;
            Lamp::random_device<uint32_t> rand(34567);

            for (size_t i = 0; i < vcount; ++i) {
                v.push_back(rand.RandIntBetween(-5000, 5000));
            }
            TimeStamp::Start();
            Lamp::sort<Lamp::SortStrat::HeapAscending>(v.begin(), v.end());
            TimeStamp::End();
            auto duration = TimeStamp::Duration();
            std::cout << duration << std::endl;
        }

        // std::sort(Introsort?)
        std::cout << "std::sort : ";
        {
            Lamp::Vector<int> v;
            Lamp::random_device<uint32_t> rand(34567);

            for (size_t i = 0; i < vcount; ++i) {
                v.push_back(rand.RandIntBetween(-5000, 5000));
            }
            TimeStamp::Start();
            std::sort(v.begin(), v.end());
            TimeStamp::End();
            auto duration = TimeStamp::Duration();
            std::cout << duration << std::endl;
        }

        // Quick sort is faster when n is smaller(I think it happens because of the depth).
    }

    // Max-min heap implementation
    if (false) {
        Lamp::Vector<int> v;
        Lamp::random_device<uint32_t> rand(34567);

        constexpr size_t vcount = 900000;

        for (size_t i = 0; i < vcount; ++i) {
            v.push_back(rand.RandIntBetween(-5000, 5000));
        }
        //          0
        //      1       2
        //   3    4   5   6
        // 7  8  9


        Lamp::make_heap<int>(v.data(), v.size());
        //PrintVector(v);

        for (size_t i = 0; i < v.size(); ++i) {
            const auto left = 2 * i+1;
            const auto right = 2 * i+2;

            if ((left < v.size() && v[i] < v[left]) || (right < v.size() && v[i] < v[right])) {
                std::cout << "Failed max heap test in v[" << i  << "] = " << v[i] <<
                    ", left,right : " << v[left] << ' ' << v[right] << std::endl;
                break;
            }
            else if (v[i] > v[0]) {
                std::cout << "Failed max heap property." << std::endl;
            }
        }
        std::cout << "max : " << v[0] << std::endl;

        Lamp::make_heap<int, Lamp::HeapStrat::Min>(v.data(), v.size());
        //PrintVector(v);

        for (size_t i = 0; i < v.size(); ++i) {
            const auto left = 2 * i+1;
            const auto right = 2 * i+2;

            if ((left < v.size() && v[i] > v[left]) || (right < v.size() && v[i] > v[right])) {
                std::cout << "Failed min heap test in v[" << i  << "] = " << v[i] <<
                    ", left,right : " << v[left] << ' ' << v[right] << std::endl;
                break;
            }
            else if (v[i] < v[0]) {
                std::cout << "Failed min heap property." << std::endl;
            }
        }
        std::cout << "min : " << v[0] << std::endl;
    }

    if (true) {
        std::cout << "Heap sort implementation test" << std::endl;
        Lamp::random_device<uint32_t> rand(34567);

        constexpr size_t vcount = 10;

        for (size_t i = 0; i < vcount; ++i) {
            Lamp::Vector<int> v;
            for (size_t j = 0; j < i; ++j) {
                v.push_back(rand.RandIntBetween(-5000, 5000));
            }

            Lamp::sort<Lamp::SortStrat::HeapAscending>(v.begin(), v.end());

            for (size_t j = 1; j < i; ++j) {
                if (v[j -1] > v[j]) {
                    std::cout << "Lamp::Vector not sorted properly." << std::endl;
                    PrintVector(v);
                    break;
                }
            }
        }
    }

    if (false) {
        std::cout << "Insertion sort implementation test" << std::endl;
        Lamp::random_device<uint32_t> rand(34567);

        Lamp::Vector<int> v;

        v.push_back(5);
        v.push_back(4);
        v.push_back(3);

        std::cout << "Before sort : " << std::endl;
        PrintVector(v);

        Lamp::sort<Lamp::SortStrat::InsertionAscending>(v.begin(), v.end());

        for (size_t j = 1; j < v.size(); ++j) {
            if (v[j -1] > v[j]) {
                std::cout << "Lamp::Vector not sorted properly." << std::endl;
                PrintVector(v);
                break;
            }
        }
    }

    if (true) {
        std::cout << "Insertion sort implementation test" << std::endl;
        Lamp::random_device<uint32_t> rand(34567);

        constexpr size_t vcount = 1000;
        bool is_all_pass = true;
        for (size_t i = 0; i < vcount; ++i) {
            Lamp::Vector<int> v;
            for (size_t j = 0; j < i; ++j) {
                v.push_back(rand.RandIntBetween(-5000, 5000));
            }

            Lamp::sort<Lamp::SortStrat::InsertionAscending>(v.begin(), v.end());

            for (size_t j = 1; j < i; ++j) {
                if (v[j -1] > v[j]) {
                    std::cout << "Lamp::Vector not sorted properly." << std::endl;
                    PrintVector(v);
                    is_all_pass = false;
                    break;
                }
            }
        }

        if (is_all_pass) {
            std::cout << "All pass for insertion sort implementation test." << std::endl;
        }
    }




    std::cout << std::endl;
    return 0;
}
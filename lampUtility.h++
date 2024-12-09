
#ifndef LAMPUTILITY_H
#define LAMPUTILITY_H

namespace Lamp {

    enum Endianness {
        LittleEndian, BigEndian, Count
    };

    template<typename T>
    Endianness CheckEndianness() {
        T end = static_cast<T>(1);
        uint8_t endianness = reinterpret_cast<uint8_t>(reinterpret_cast<uint8_t*>(&end)[0]);

        T if_big_endian = 1 << ((sizeof(T) * 8) - 1);

        if (endianness == 1) {
            return LittleEndian;
        }
        else if (if_big_endian == 1) {
            return BigEndian;
        }
        else {
            // TODO: runtime error
        }
        return Count;
    }

    template<typename T>
    bool IsLittleEndian() {
        return CheckEndianness<T>() == LittleEndian;
    }
}

#endif //LAMPUTILITY_H

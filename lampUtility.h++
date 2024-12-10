
#ifndef LAMPUTILITY_H
#define LAMPUTILITY_H

#include <cstdint>

namespace Lamp {

    enum Endianness {
        LittleEndian, BigEndian, Count
    };

    template<typename T> T Clamp(const T & _src, const T & _min, const T & _max) {
        T result = _src;

        if (result < _min) {
            result = _min;
        }
        else if (result > _max) {
            result = _max;
        }

        return result;
    }

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

        return Count;
    }

    template<typename T>
    bool IsLittleEndian() {
        return CheckEndianness<T>() == LittleEndian;
    }
}

#endif //LAMPUTILITY_H

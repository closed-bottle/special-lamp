#ifndef LAMPUTILITY_H
#define LAMPUTILITY_H

#include <cstdint>

namespace Lamp {
    enum class Endianness {
        Invalid, Little, Big, Count
    };

    template<typename T>
    T Clamp(const T &_src, const T &_min, const T &_max) {
        T result = _src;

        if (result < _min) {
            result = _min;
        } else if (result > _max) {
            result = _max;
        }

        return result;
    }

    template<typename T>
    Endianness CheckEndianness() {
        T end = static_cast<T>(1);
        uint8_t endianness = reinterpret_cast<uint8_t>(reinterpret_cast<uint8_t *>(&end)[0]);

        T if_big_endian = 1 << ((sizeof(T) * 8) - 1);

        if (endianness == 1) {
            return Endianness::Little;
        } else if (if_big_endian == 1) {
            return Endianness::Big;
        }

        return Endianness::Invalid;
    }

    template<typename T>
    bool IsLittleEndian() {
        return CheckEndianness<T>() == Endianness::Little;
    }

    template<typename T>
    void Swap(T& _lhs, T& _rhs) {
        T temp = std::move(_lhs);
        _lhs = std::move(_rhs);
        _rhs = std::move(temp);
    }
}

#endif //LAMPUTILITY_H

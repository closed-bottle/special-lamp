#ifndef LAMPCRC32_H
#define LAMPCRC32_H
#include <cstdint>

/*
 * Painless guide to CRC
 * http://www.ross.net/crc/download/crc_v3.txt
 * https://github.com/Michaelangel007/crc32/blob/master/README.md
 *
 *
 * How to calculate CRC32
 * 0.Initialize crc buffer with 1s(0xFFFFFFFF) so we can output something other than 0
 * When we have input of  "0". I think it can be anything it would be better to stick with the standard.
 * 1. Append 0s to the message (in this case, << 24 part because we are take 8 bits as input)
 * and output 32 bits.)
 * 2. Do modulo 2, or XOR between Polynomial value and message.
 *  2 - 1. If MSB (endianness doesn't matter at all for this problem, it is already loaded) is 1
 *         Shift it to the left and do modulo 2,
 *  2 - 2. Else, it is not a number (000010101 is same with 10101.),
 *         shift it.
 * 3. If we reached to the end (that is, original bits are pushed away except for remainders),
 *   rest of numbers are remainder.
 *
 * I'm not sure why we have to reverse CRC before return, but it might be part of specification or
 * just to avoid returning all 0s since we started with all 1s.
 */

// These macros are intended to use

#ifndef LAMP_NORMAL_CRC32
#ifndef LAMP_REVERSED_CRC32
#define LAMP_REVERSED_CRC32
#endif
#endif

#define LAMP_LUT_CRC32

#define LAMP_PRECOMPILED_LUT_CRC32

#ifndef LAMP_PRECOMPILED_LUT_CRC32
#include <iostream>
#endif


namespace Lamp {
    class CRC32 {
#ifndef LAMP_PRECOMPILED_LUT_CRC32
        uint32_t* crc32_table_ = nullptr;
#else
#include "lampCRC32LUT.inl"
#endif
        public:
#ifndef LAMP_PRECOMPILED_LUT_CRC32
        ~CRC32() {
            if (crc32_table_)
                delete[] crc32_table_;
        }
        void OutputTable();
        void GenerateCRC32LUT();
#endif

        uint32_t GetCRC32(const uint8_t * _data, size_t _size);


        template <typename T>
        static T ReverseBits(T _rhs);
#ifdef LAMP_NORMAL_CRC32
        static constexpr uint32_t POLY = 0x04C11DB7;
#endif
#ifdef LAMP_REVERSED_CRC32
        static constexpr uint32_t POLY = 0xEDB88320;
#endif
    };

    template<typename T>
    T CRC32::ReverseBits(T _rhs) {
        T reversed = 0;

        for (uint8_t i = 0; i < sizeof(T) * 8; i++) {
            reversed <<= 1;

            if (_rhs & 1) {
                reversed |= 1;
            }

            _rhs >>= 1;
        }

        return reversed;
    }

#ifdef LAMP_NORMAL_CRC32
    inline uint32_t CRC32::GetCRC32(const uint8_t *_data, size_t _size) {
#ifdef LAMP_LUT_CRC32
        uint32_t crc = 0xFFFFFFFF;
        while (_size--) {
            crc = crc32_table_[((crc >> 24) ^ ReverseBits(*_data++)) & 0xFF] ^ (crc << 8);
        }

        return ReverseBits<uint32_t>(~crc);
#else
        // Normal version, I cant see any benefit from this one.
        uint32_t crc = 0xFFFFFFFF;

        while (_size--) {
            crc ^= (ReverseBits<uint8_t>(*_data++) << 24);

            for (uint8_t i = 0; i < 8; i++) {
                if (crc & 0x80000000) {
                    crc = (crc << 1) ^ POLY;
                }
                else {
                    crc = (crc << 1);
                }
            }
        }

        return ReverseBits<uint32_t>(~crc);
#endif
    }
#ifdef LAMP_LUT_CRC32
#ifndef LAMP_PRECOMPILED_LUT_CRC32
    inline void CRC32::GenerateCRC32LUT() {
        uint32_t crc = 0xFFFFFFFF;
        crc32_table_ = new uint32_t[256];

        for (uint16_t i = 0; i < 256; i++) {
            crc = i << 24;

            for (uint8_t i = 0; i < 8; i++) {
                if (crc & 0x80000000) {
                    crc = (crc << 1) ^ POLY;
                }
                else {
                    crc = (crc << 1);
                }
            }

            crc32_table_[i] = crc;
        }
    }
#endif
#endif
#endif
#ifdef LAMP_REVERSED_CRC32
    inline uint32_t CRC32::GetCRC32(const uint8_t *_data, size_t _size) {
#ifdef LAMP_LUT_CRC32
        uint32_t crc = 0xFFFFFFFF;
        while (_size--) {
            crc = crc32_table_[(crc ^ (*_data++)) & 0xFF] ^ (crc >> 8);
        }

        return ~crc;
#else
        uint32_t crc = 0xFFFFFFFF;

        while (_size--) {
            crc ^= (*_data++);

            for (uint8_t i = 0; i < 8; i++) {
                if (crc & 1) {
                    crc = (crc >> 1) ^ POLY;
                }
                else {
                    crc = (crc >> 1);
                }
            }

        }

        return ~crc;
#endif
    }
#ifdef LAMP_LUT_CRC32
#ifndef LAMP_PRECOMPILED_LUT_CRC32
    inline void CRC32::GenerateCRC32LUT() {
        uint32_t crc = 0xFFFFFFFF;

        crc32_table_ = new uint32_t[256];

        for (uint16_t i = 0; i < 256; i++) {
            crc = i;

            for (uint8_t i = 0; i < 8; i++) {
                if (crc & 1) {
                    crc = (crc >> 1) ^ POLY;
                }
                else {
                    crc = (crc >> 1);
                }
            }

            crc32_table_[i] = crc;
        }
    }
#endif
#endif
#endif

#ifndef LAMP_PRECOMPILED_LUT_CRC32
    inline void CRC32::OutputTable() {
        std::cout << "constexpr uint32_t crc32_table_[256] = {" << std::endl;

        for (uint16_t i = 0; i < 256; i++) {
            std::cout << std::hex << "0x" << crc32_table_[i] << ", ";

            if (i % 5 == 0) {
                std::cout << std::endl;
            }
        }

        std::cout << "};" << std::endl;
    }
#endif


}

#endif //LAMPCRC32_H

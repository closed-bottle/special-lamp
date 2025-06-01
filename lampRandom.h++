//
// Created by JJ on 5/30/2025.
//

#ifndef LAMPRANDOM_H
#define LAMPRANDOM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wshift-count-overflow"

#include <chrono>

#include "lampUtility.h++"

// This is p-random, so it should never be used for some security reason.

namespace Lamp {
    enum LCGType {
        LCGinvalid,
        LCGrandqd1,
        LCGjava,
        LCGborland,
        LCGglibc,
        LCGansi,
        LCGparkmiller
    };

    // I don't think anything else than uint32_t will make any meaningful difference,
    // but I still try to do template for best practice.(uint16_t for example.)
    template<typename poolType>
    class random_device {
        static constexpr size_t poolSize_ = sizeof(poolType);
        static constexpr size_t PoolSizeInBit() {
            return poolSize_ * 8;
        }

        poolType entropyPool_ = 1;
        LCGType lcgType_ = LCGparkmiller;

        // 0 = always, 0xFFFFFFF(seven Fs.) = never.
        uint16_t refreshRate_ = 32;
        uint16_t refreshCounter_ = 0;

        // Use ASLR
        void CollectAddressEntropy() {
            volatile char c0;
            volatile char c1;
            volatile char c2;
            volatile char c3;

            static constexpr size_t addrSize = sizeof(&c0);

            // Assume that maximum size of pool is 64bit, as well as address.
            // Minimum address size will be 16 bit, which is very unlikely to be happened.
            entropyPool_ ^= reinterpret_cast<poolType>(&c0);
            entropyPool_ = entropyPool_ << addrSize;
            entropyPool_ ^= reinterpret_cast<poolType>(&c1);
            entropyPool_ = entropyPool_ << addrSize;
            entropyPool_ ^= reinterpret_cast<poolType>(&c2);
            entropyPool_ = entropyPool_ << addrSize;
            entropyPool_ ^= reinterpret_cast<poolType>(&c3);
            entropyPool_ = entropyPool_ << addrSize;
        }

        void CollectTimingEntropy() {
            auto timeline = std::chrono::system_clock::now().time_since_epoch().count();
            entropyPool_ ^= timeline;
            entropyPool_ = (entropyPool_ << 1) | (entropyPool_ >> (PoolSizeInBit() - 1));
        }

        void RefreshTiming() {
            if (refreshRate_ != 0xFFFFFFF && refreshCounter_++ >= refreshRate_) {
                CollectTimingEntropy();
                refreshCounter_ = 0;
            }
        }

    public:

        // https://en.wikipedia.org/wiki/Linear_congruential_generator
        // https://en.wikipedia.org/wiki/Lehmer_random_number_generator
        // Linear congruential generator, think it as some kind of clock, which can be wind multiple times
        // so it seems random.
        template<LCGType type = LCGparkmiller>
        poolType LCGStep() {
            unsigned long long a = 0;
            unsigned long long c = 0;
            unsigned msb = 0;
            unsigned lsb = 0;


            switch (type) {
                case LCGrandqd1:
                    a = 1664525UL; c = 1013904223UL;
                    msb = PoolSizeInBit() -1; lsb = 0;
                    break;
                case LCGjava:
                    a = 25214903917UL; c = 11UL;
                    msb = 47; lsb = 17;
                    break;
                case LCGborland:
                    a = 22695477UL; c = 1UL;
                    msb = 30; lsb = 16;
                    break;
                case LCGglibc:
                    a = 1103515245UL; c = 12345UL;
                    msb = 30; lsb = 0;
                    break;
                case LCGansi:
                    a = 1103515245UL; c = 12345UL;
                    msb = 30; lsb = 16; // same with glib, but different bits
                    break;
                case LCGparkmiller:
                    LAMPASSERT(false, "This line should never be reached, parkmiller implementation is down below.");
                    break;
            }

            poolType new_entropy = entropyPool_ * a + c;
            const uint8_t bit_count = msb - lsb + 1;
            const poolType mask = ((1 << msb) ^ ((1 << msb) - 1) ) & ~((1 << lsb) - 1);

            for (uint8_t i = 0; i < PoolSizeInBit(); i += bit_count) {
                new_entropy = (new_entropy * a + c) & mask;
                // wrap around
                new_entropy = (new_entropy << bit_count) | (new_entropy >> (PoolSizeInBit() - bit_count));
            }

            entropyPool_ = new_entropy;
            return entropyPool_;
        }

        template<>
        poolType LCGStep<LCGparkmiller>() {
            // Schrage's method to use only 32bit operations for better compatibility.
            // There is 16bit arithmetic version, but I was not able to understand it...
            constexpr uint32_t m = 0x7fffffff;
            constexpr uint32_t a = 48271UL;
            constexpr uint32_t q = m / a;
            constexpr uint32_t r = m % a;

            constexpr uint8_t bit_count = 32;

            poolType new_entropy = entropyPool_ ;

            for (int i = 0; i < PoolSizeInBit(); i += bit_count) {
                new_entropy = new_entropy % 0x7fffffff;
                new_entropy += new_entropy == 0 ? 1 : 0;

                uint32_t div = new_entropy / q;
                uint32_t rem = new_entropy % q;

                uint32_t s = rem * a;
                uint32_t t = div * r;
                volatile uint32_t result = s - t;

                if (result < 0)
                    result += m;

                new_entropy = result;
                new_entropy = (new_entropy << bit_count) | (new_entropy >> (PoolSizeInBit() - bit_count));
            }


            entropyPool_ = new_entropy;
            return entropyPool_;
        }

        random_device() {
            InitializeEntropy();
        }

        void InitializeEntropy() {
            CollectAddressEntropy();

            for (int i = 0; i < 16; ++i) {
                CollectTimingEntropy();
                LCGStep<LCGparkmiller>();
            }
        }

        poolType XORShift32StarStep() {
            uint64_t x = static_cast<uint64_t>(entropyPool_);
            x += x == 0 ? 1 : 0;

            x ^= x >> 12;
            x ^= x << 25;
            x ^= x >> 27;
            x *= 0x2545F4914F6CDD1DULL;

            entropyPool_ = static_cast<poolType>(x);

            return entropyPool_;
        }

        poolType Rand() {
            RefreshTiming();
            return XORShift32StarStep();
        }

        poolType RandRange(poolType _max) {
            RefreshTiming();
            return XORShift32StarStep() % _max;
        }

        poolType RandBetween(poolType _min, poolType _max) {
            RefreshTiming();
            return _min + RandRange(_max - _min);
        }

        bool RandBool() {
            RefreshTiming();
            return Rand() & 1;
        }

        float RandFloat(uint32_t resolution) {
            RefreshTiming();
            const float f = RandRange(resolution) / static_cast<float>(resolution);
            return f;
        }

        int RandIntBetween(int _min, int _max) {
            RefreshTiming();
            return _min + RandRange(_max - _min);
        }

        float RandNormalizedFloat(uint32_t resolution) {
            RefreshTiming();
            const float f = RandIntBetween(-resolution, resolution) / static_cast<float>(resolution);
            return f;
        }

        template<typename T>
        void Shuffle(T* _data, const size_t _count) {
            for (size_t i = 0; i < _count; ++i) {
                Lamp::Swap(_data[i], _data[RandRange(_count)]);
            }
        }

        void SetRefreshRate(uint16_t _in) {
            refreshRate_ = _in;
            refreshCounter_ = 0;
            CollectTimingEntropy();
        }
    };
}
#pragma GCC diagnostic pop
#endif //LAMPRANDOM_H

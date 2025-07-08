//
// Created by JJ on 7/5/2025.
//

#ifndef LAMPHEAP_H
#define LAMPHEAP_H

#include "lampVector.h++"
#include "lampAlgorithm.h++"
#include "lampAssert.h++"

namespace {
    template<typename T, Lamp::HeapStrat strat = Lamp::HeapStrat::Invalid>
    struct HeapCompareUtil {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            LAMPASSERT(false, "Invalid HeapStrat for heap");
            return false;
        }
    };

    template<typename T>
    struct HeapCompareUtil<T, Lamp::HeapStrat::Min> {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs > _rhs;
        }
    };

    template<typename T>
    struct HeapCompareUtil<T, Lamp::HeapStrat::Max> {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs < _rhs;
        }
    };
}

namespace Lamp {
    template<typename T, HeapStrat strat>
    class Heap {
        Vector<T> vector;

    public:
        T& top() {
            return vector[0];
        }

        bool empty() const {
            return vector.empty();
        }

        size_t size() const {
            return vector.size();
        }

        void push(const T& _in) {
            vector.push_back(_in);

            size_t i = vector.size() - 1;
            size_t p = (i -1) / 2;

            while (i != 0 && HeapCompareUtil<T, strat>::Compare(vector[p], vector[i])) {
                Swap(vector[i], vector[p]);

                i = p;
                p = (i -1) / 2;
            }
        }

        template<typename... Args>
        void push_range(Args&&... _args);
        void emplace();
        void pop();

        void shrink_to_fit() {
            vector.shrink_to_fit();
        }
    };
};

#endif //LAMPHEAP_H

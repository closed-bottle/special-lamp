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

        // Note that these heapify* is same one from Algorithm.h++ but forced to start with root.
        // Heapify starting from leaf.
        void heapifyLeaf() {
            size_t i = vector.size() - 1;
            size_t p = (i -1) / 2;

            while (i != 0 && HeapCompareUtil<T, strat>::Compare(vector[p], vector[i])) {
                Swap(vector[i], vector[p]);

                i = p;
                p = (i -1) / 2;
            }
        }

        // heapify starting from root
        void heapifyRoot() {
            bool is_heap = false;
            size_t i = 0;
            const size_t count = vector.size();

            while (!is_heap) {
                auto left = (2 * i) + 1;
                auto right = (2 * i) + 2;
                auto peak = i;


                if (left < count && HeapCompareUtil<T, strat>::Compare(vector[peak], vector[left])) {
                    peak = left;
                }
                if (right < count && HeapCompareUtil<T, strat>::Compare(vector[peak], vector[right])) {
                    peak = right;
                }


                is_heap = peak == i;
                if (!is_heap) {
                    Lamp::Swap(vector[i], vector[peak]);
                    i = peak;
                }
            }
        }

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
            heapifyLeaf();
        }

        template<typename... Args>
        void push_range(T _first, Args&&... _args) {
            push(_first);
            push_range(_args...);
        }

        void push_range(T _last) {
            push(_last);
        }

        template<typename... Args>
        void emplace(Args&&... _args) {
            vector.emplace_back(_args...);
            heapifyLeaf();
        }

        void pop() {
            Swap(vector[0], vector[vector.size() -1]);
            vector.pop_back();

            if (!vector.empty())
                heapifyRoot();
        }

        void shrink_to_fit() {
            vector.shrink_to_fit();
        }
    };
};

#endif //LAMPHEAP_H

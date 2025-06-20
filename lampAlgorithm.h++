//
// Created by JJ on 6/15/2025.
//

#ifndef LAMPALGORITHM_H
#define LAMPALGORITHM_H

namespace Lamp {
    enum SortStrat {
        SortInvalid = 0,
        SortBubbleAscending,
        SortQuickAscending,
        SortCount
    };
}

namespace {
    template<typename itr_type, Lamp::SortStrat strat>
    struct SortHelper {
        static void sort(itr_type& _begin, itr_type& _end) {
            LAMPASSERT(false, "Invalid SortStrat.");
        }
    };

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortBubbleAscending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            while (_begin != _end) {
                itr_type left = _begin;
                // + operator would most likely not implemented.
                itr_type next = left; // Only reason copying "left" is because on most iterators,
                                      // default constructor is deleted for safety.
                {
                    itr_type temp = left;
                    ++temp;
                    next = temp;
                }

                while (next != _end) {
                    if (*left > *next) {
                        Lamp::Swap(*left, *next);
                    }

                    ++left;
                    ++next;
                }

                _end = left;
            }
        }
    };

    template<typename itr_type>
    itr_type QuickPartition(itr_type _low, itr_type _high) {
        // Few points to note:
        // 1. It only works with C-style data set if operator < is not implemented.
        //  it is not implemented for vector, but it is guaranteed to work with address from C-style data set.
        // 2. I can only use first(_low) iterator as pivot, because _end is not an actual value, but a marker.

        itr_type pivot = _low;
        itr_type i = _low++; // must be same with initial _low, so it can be out of bound. we can use condition
                             // i == _low as nullptr.
        itr_type j = _low;


        while (j != _high) {
            if ((*j) <= *pivot) {
                ++i;

                // Again, < operator needs to be implemented or this one should be C style array.
                if (i < j) {
                    Lamp::Swap(*i, *j);
                }
            }
            ++j;
        }

        Lamp::Swap(*i, *pivot);
        Lamp::Swap(i, pivot);


        return pivot;
    }

    template<typename itr_type>
    void QuickSort(itr_type _begin, itr_type _end) {

        Lamp::stack<Lamp::pair<itr_type, itr_type>> stck;
        stck.emplace(_begin, _end);

        while (!stck.empty()) {
            auto curr = stck.top();
            stck.pop();

            if (curr.first < curr.second) {
                itr_type pivot = QuickPartition(curr.first, curr.second);
                stck.emplace(curr.first, pivot);
                stck.emplace(++pivot, curr.second);
            }
        }
    }


    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortQuickAscending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            QuickSort(_begin, _end);
        }
    };

    template<typename T>
    void heapifyAux(T* _arr, size_t _count, size_t _i) {
        static auto get_left_child = [](const size_t& _index) -> size_t {
            return 2*_index+1;
        };

        static auto get_right_child = [&](const size_t& _index) -> size_t {
            return 2*_index+2;
        };

        auto left = get_left_child(_i);
        auto right = get_right_child(_i);
        auto maximum = _i;

        if (left < _count && _arr[left] > _arr[maximum]) {
            maximum = left;
        }
        if (right < _count && _arr[right] > _arr[maximum]) {
            maximum = right;
        }

        if (maximum != _i) {
            Lamp::Swap(_arr[_i], _arr[maximum]);
            heapifyAux(_arr, _count, maximum);
        }
    }
}


namespace Lamp {
    template<SortStrat strat>
    void sort(auto _begin, auto _end) {
        SortHelper<decltype(_begin), strat>::sort(_begin, _end);
    }

    template<typename T>
    void make_heap(T* _arr, size_t _count) {
        const size_t _stride = sizeof(T);

        for (size_t i = 1; i <= _count; ++i) {
            heapifyAux(_arr, _count, (_count - i));
        }
    }
};

#endif //LAMPALGORITHM_H

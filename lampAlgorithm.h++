//
// Created by JJ on 6/15/2025.
//

#ifndef LAMPALGORITHM_H
#define LAMPALGORITHM_H

namespace Lamp {
    enum class SortStrat {
        Invalid = 0,
        BubbleAscending,
        QuickAscending,
        HeapAscending,
        HeapDescending,
        Count
    };

    enum class HeapStrat {
        Invalid = 0,
        Max,
        Min,
        Count
    };

    template<typename T, HeapStrat>
    void make_heap(T* _arr, size_t _count);
}

namespace {
    template<typename itr_type, Lamp::SortStrat strat>
    struct SortHelper {
        static void sort(itr_type& _begin, itr_type& _end) {
            LAMPASSERT(false, "Invalid SortStrat.");
        }
    };

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::BubbleAscending> {
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
    struct SortHelper<itr_type, Lamp::SortStrat::QuickAscending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            QuickSort(_begin, _end);
        }
    };




    template<typename T, Lamp::HeapStrat strat = Lamp::HeapStrat::Invalid>
    struct HeapUtil {
        static bool Compare(const T& _lhs, const T& _rhs) {
            LAMPASSERT(false, "Invalid HeapStrat");
            return false;
        }
    };
    template<typename T>
    struct HeapUtil<T, Lamp::HeapStrat::Max> {
        static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs > _rhs;
        }
    };

    template<typename T>
    struct HeapUtil<T, Lamp::HeapStrat::Min> {
        static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs < _rhs;
        }
    };

    template<typename T, Lamp::HeapStrat strat>
    void heapifyAux(T* _arr, size_t _count, size_t _i) {
        static auto get_left_child = [](const size_t& _index) -> size_t {
            return 2*_index+1;
        };

        static auto get_right_child = [](const size_t& _index) -> size_t {
            return 2*_index+2;
        };

        auto left = get_left_child(_i);
        auto right = get_right_child(_i);
        auto peak = _i; // It is Maximum or Minimum basted on strat

        if (left < _count && HeapUtil<T, strat>::Compare(_arr[left], _arr[peak])) {
            peak = left;
        }
        if (right < _count && HeapUtil<T, strat>::Compare(_arr[right], _arr[peak])) {
            peak = right;
        }

        if (peak != _i) {
            Lamp::Swap(_arr[_i], _arr[peak]);
            heapifyAux<T, strat>(_arr, _count, peak);
        }
    }

    template<typename itr_type, Lamp::SortStrat strat>
    struct HeapSortHelper {
        static void sort(itr_type& _begin, itr_type& _end) {
            // For heap sort, it only takes pointer type, because it needs random access.
            // It is possible to implement random accessible iterator, but it will be much slower
            // due to indirection & cache miss.
            size_t count = (_end - _begin);

            if (count < 2)
                return;

            constexpr Lamp::HeapStrat heap_strat = strat == Lamp::SortStrat::HeapAscending ?
                Lamp::HeapStrat::Max : Lamp::HeapStrat::Min;

            Lamp::make_heap<std::decay_t<decltype(*_begin)>, heap_strat>(_begin, count);
            Lamp::Swap(_begin[0], _begin[count - 1]);


            while (count > 1) {
                heapifyAux<std::decay_t<decltype(*_begin)>, heap_strat>(_begin, --count, 0);
                Lamp::Swap(_begin[0], _begin[count - 1]);
            }
        }
    };

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::HeapAscending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            HeapSortHelper<itr_type, Lamp::SortStrat::HeapAscending>::sort(_begin, _end);
        }
    };

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::HeapDescending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            HeapSortHelper<itr_type, Lamp::SortStrat::HeapDescending>::sort(_begin, _end);
        }
    };
}


namespace Lamp {
    template<SortStrat strat>
    void sort(auto _begin, auto _end) {
        SortHelper<decltype(_begin), strat>::sort(_begin, _end);
    }

    template<typename T, HeapStrat strat = HeapStrat::Max>
    void make_heap(T* _arr, size_t _count) {
        for (size_t i = 1; i <= _count; ++i) {
            heapifyAux<T, strat>(_arr, _count, (_count - i));
        }
    }
};

#endif //LAMPALGORITHM_H

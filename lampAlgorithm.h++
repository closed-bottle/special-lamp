//
// Created by JJ on 6/15/2025.
//

#ifndef LAMPALGORITHM_H
#define LAMPALGORITHM_H

#include "lampUtility.h++"

namespace Lamp {
    enum class SortStrat {
        Invalid = 0,
        BubbleAscending,
        QuickAscending,
        QuickDescending,
        HeapAscending,
        HeapDescending,
        InsertionAscending,
        InsertionDescending,
        IntroAscending,
        IntroDescending,
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


    template<typename T, Lamp::SortStrat strat = Lamp::SortStrat::Invalid>
    struct QuickSortUtil {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            LAMPASSERT(false, "Invalid SortStrat");
            return false;
        }
    };

    template<typename T>
    struct QuickSortUtil<T, Lamp::SortStrat::QuickDescending> {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs > _rhs;
        }
    };

    template<typename T>
    struct QuickSortUtil<T, Lamp::SortStrat::QuickAscending> {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs < _rhs;
        }
    };


    template<typename itr_type, Lamp::SortStrat strat>
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
            if (QuickSortUtil<typename Lamp::RemovePtr<itr_type>::type, strat>::Compare((*j), *pivot)) {
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

    template<typename itr_type, Lamp::SortStrat strat>
    void QuickSort(itr_type _begin, itr_type _end) {

        Lamp::stack<Lamp::pair<itr_type, itr_type>> stck;
        stck.emplace(_begin, _end);

        while (!stck.empty()) {
            auto curr = stck.top();
            stck.pop();

            if (curr.first < curr.second) {
                itr_type pivot = QuickPartition<itr_type, strat>(curr.first, curr.second);
                stck.emplace(curr.first, pivot);
                stck.emplace(++pivot, curr.second);
            }
        }
    }

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::QuickAscending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            QuickSort<itr_type, Lamp::SortStrat::QuickAscending>(_begin, _end);
        }
    };

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::QuickDescending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            QuickSort<itr_type, Lamp::SortStrat::QuickDescending>(_begin, _end);
        }
    };


    template<typename T, Lamp::HeapStrat strat = Lamp::HeapStrat::Invalid>
    struct HeapSortUtil {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            LAMPASSERT(false, "Invalid HeapStrat");
            return false;
        }
    };
    template<typename T>
    struct HeapSortUtil<T, Lamp::HeapStrat::Max> {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs > _rhs;
        }
    };

    template<typename T>
    struct HeapSortUtil<T, Lamp::HeapStrat::Min> {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs < _rhs;
        }
    };

    template<typename T, Lamp::HeapStrat strat>
    void heapify(T* _arr, size_t _count, size_t _i) {
        bool is_heap = false;

        while (!is_heap) {
            auto left = (2 * _i) + 1;
            auto right = (2 * _i) + 2;
            auto peak = _i; // It is Maximum or Minimum basted on strat


            if (left < _count && HeapSortUtil<T, strat>::Compare(_arr[left], _arr[peak])) {
                peak = left;
            }
            if (right < _count && HeapSortUtil<T, strat>::Compare(_arr[right], _arr[peak])) {
                peak = right;
            }


            is_heap = peak == _i; // It is a heap if parent(_i) node satisfy the all the heap property.
                                  // which keep the _i unchanged.
            if (!is_heap) {
                Lamp::Swap(_arr[_i], _arr[peak]);
                _i = peak;
            }
        }


        // Recursive versoin was implemented :
        // cfa83eca8dc051ae8812ad98e16dd94ba73bc414
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

            Lamp::make_heap<typename Lamp::RemovePtr<itr_type>::type, heap_strat>(_begin, count);
            Lamp::Swap(_begin[0], _begin[count - 1]);


            while (count > 1) {
                heapify<typename Lamp::RemovePtr<itr_type>::type, heap_strat>(_begin, --count, 0);
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

    template<typename T, Lamp::SortStrat strat = Lamp::SortStrat::Invalid>
    struct InsertSortUtil {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            LAMPASSERT(false, "Invalid SortStrat");
            return false;
        }
    };

    template<typename T>
    struct InsertSortUtil<T, Lamp::SortStrat::InsertionAscending> {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs > _rhs;
        }
    };

    template<typename T>
    struct InsertSortUtil<T, Lamp::SortStrat::InsertionDescending> {
        constexpr static bool Compare(const T& _lhs, const T& _rhs) {
            return _lhs < _rhs;
        }
    };

    template<typename itr_type, Lamp::SortStrat strat>
    struct InsertSortHelper {
        static void sort(itr_type& _begin, itr_type& _end) {
            // Start with _begin instead of ++_begin so we can filter out _begin == _end.
            auto right = _begin;

            while (right != _end) {
                auto left = _begin;
                auto insert = right;

                while (left != right) {
                    if (InsertSortUtil<typename Lamp::RemovePtr<itr_type>::type, strat>
                        ::Compare(*left, *insert)) {
                        // Shift
                        auto i = left;
                        auto j = left;
                        ++j;
                        auto cache = insert;

                        while (j != insert) {
                            Lamp::Swap(*i, *cache);
                            ++i;
                            ++j;
                        }
                        Lamp::Swap(*i, *cache);
                        break;
                    }
                    ++left;
                }
                ++right;
            }
        }
    };

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::InsertionAscending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            InsertSortHelper<itr_type, Lamp::SortStrat::InsertionAscending>::sort(_begin, _end);
        }
    };

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::InsertionDescending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            InsertSortHelper<itr_type, Lamp::SortStrat::InsertionDescending>::sort(_begin, _end);
        }
    };

    template<typename itr_type, Lamp::SortStrat insert_strat, Lamp::SortStrat heap_strat, Lamp::SortStrat quick_strat>
    struct IntroSortHelper {
        static void sort(itr_type& _begin, itr_type& _end) {
            size_t count = _end - _begin;
            size_t depth_limit = std::log(count);

            Lamp::stack<Lamp::pair<itr_type, itr_type>> stck;
            stck.emplace(_begin, _end);

            while (!stck.empty()) {
                auto curr = stck.top();
                stck.pop();

                if (curr.second - curr.first < 16) {
                    // insertion sort
                    SortHelper<itr_type, insert_strat>::sort(curr.first, curr.second);
                }
                else if (depth_limit == 0) {
                    // heap sort
                    SortHelper<itr_type, heap_strat>::sort(curr.first, curr.second);
                }
                else {
                    --depth_limit;
                    if (curr.first < curr.second) {
                        itr_type pivot
                        = QuickPartition<itr_type, quick_strat>(curr.first, curr.second);
                        stck.emplace(curr.first, pivot);
                        stck.emplace(++pivot, curr.second);
                    }
                }
            }
        }
    };



    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::IntroAscending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            IntroSortHelper<itr_type,
            Lamp::SortStrat::InsertionAscending,
            Lamp::SortStrat::HeapAscending,
            Lamp::SortStrat::QuickAscending>::sort(_begin, _end);
        }
    };

    template<typename itr_type>
    struct SortHelper<itr_type, Lamp::SortStrat::IntroDescending> {
        static void sort(itr_type& _begin, itr_type& _end) {
            IntroSortHelper<itr_type,
            Lamp::SortStrat::InsertionDescending,
            Lamp::SortStrat::HeapDescending,
            Lamp::SortStrat::QuickDescending>::sort(_begin, _end);
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
            heapify<T, strat>(_arr, _count, (_count - i));
        }
    }
};

#endif //LAMPALGORITHM_H

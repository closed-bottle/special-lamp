//
// Created by JJ on 6/15/2025.
//

#ifndef LAMPALGORITHM_H
#define LAMPALGORITHM_H

namespace Lamp {
    enum SortStrat {
        SortInvalid = 0,
        SortBubbleAscending,
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
}


namespace Lamp {
    template<SortStrat strat>
    void sort(auto _begin, auto _end) {
        SortHelper<decltype(_begin), strat>::sort(_begin, _end);
    }
};

#endif //LAMPALGORITHM_H

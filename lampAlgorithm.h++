//
// Created by JJ on 6/15/2025.
//

#ifndef LAMPALGORITHM_H
#define LAMPALGORITHM_H


namespace {
    template<typename itr_type>
    void BubbleSortAscending(itr_type& _begin, itr_type& _end) {
        itr_type& left = _begin;

        while (left != _end) {
            itr_type begin = {};
            // + operator would most likely not implemented.
            {
                itr_type temp = left;
                ++temp;
                begin = temp;
            }
            itr_type end = _end;

            for (; begin != end; ++begin) {
                if (*left > *begin) {
                    Lamp::Swap(*left, *begin);
                }
            }

            ++left;
        }
    }
}



namespace Lamp {
    enum SortStrat {
        SortInvalid = 0,
        SortBubbleAscending ,
        SortCount
    };

    template<typename itr_type>
    void sort(itr_type _begin, itr_type _end, SortStrat strat) {
        if (strat == SortBubbleAscending) {
            BubbleSortAscending(_begin, _end);
        }
    }
};

#endif //LAMPALGORITHM_H

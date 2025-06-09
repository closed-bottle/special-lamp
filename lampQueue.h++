//
// Created by JJ on 6/9/2025.
//

#ifndef LAMPQUEUE_H
#define LAMPQUEUE_H

#include "lampDeque.h++"

namespace Lamp {
    template<typename T2, size_t segmentSize = 4, SegmentIncrement incStrat = SegmentIncDouble, size_t IncAmount = 16>
        class queue : private deque<T2, segmentSize, incStrat, IncAmount> {

        using deque = deque<T2, segmentSize, incStrat, IncAmount>;
        using Segment = typename deque::Segment;

    public:
        void push(const T2& _in) {
            deque::push_back(_in);
        }

        void pop() {
            deque::pop_front();
        }

        T2& top() {
            // If deque is not empty(have at least one element).
            Segment& curr = deque::segments_[deque::first_segment_];
            return curr.data_[(curr.start_) % segmentSize];
        }

        bool empty() const {
            return deque::total_count_ == 0;
        }

        size_t size() const {
            return deque::size();
        }
    };
}

#endif //LAMPQUEUE_H

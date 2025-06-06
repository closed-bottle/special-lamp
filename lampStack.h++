//
// Created by JJ on 6/5/2025.
//

#ifndef LAMPSTACK_H
#define LAMPSTACK_H

#include "lampDeque.h++"

namespace Lamp {
    template<typename T2, size_t segmentSize = 4, SegmentIncrement incStrat = SegmentIncDouble, size_t IncAmount = 16>
    class stack : private deque<T2, segmentSize, incStrat, IncAmount> {

        using deque = deque<T2, segmentSize, incStrat, IncAmount>;
        using Segment = typename deque::Segment;

        template<SegmentIncrement increment = SegmentIncDouble>
        void ReallocateShift() {
            deque::template IncreaseHelper<increment>();

            Segment* new_segments = new typename deque::Segment[deque::segment_capacity_];
            for (size_t i = 0; i < deque::segment_capacity_; ++i) {
                new_segments[i] = {};
                new_segments[i].data_ = new T2[segmentSize];
            }

            size_t i = 0;
            do {
                memcpy(&new_segments[i], &deque::segments_[i], sizeof(Segment));
                deque::segments_[i].data_ = nullptr;
                ++i;
            } while(i != deque::last_segment_);


            delete[] deque::segments_;
            deque::segments_ = new_segments;
        }
    public:
        void push(const T2& _in) {
            deque::push_back(_in);
        }

        void pop() {
            deque::pop_back();
        }

        T2& top() {
            // If deque is not empty(have at least one element).
            Segment& curr = deque::segments_[deque::last_segment_];
            return curr.data_[(curr.start_ + curr.count_ -1) % segmentSize];
        }

        bool empty() const {
            return deque::total_count_ == 0;
        }
    };
}

#endif //LAMPSTACK_H

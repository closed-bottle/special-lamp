//
// Created by JJ on 6/5/2025.
//

#ifndef LAMPSTACK_H
#define LAMPSTACK_H

#include "lampDeque.h++"

namespace Lamp {
    template<typename T2, size_t segmentSize = 4, SegmentIncrement incStrat = SegmentIncrement::Double, size_t IncAmount = 16>
    class stack : private deque<T2, segmentSize, incStrat, IncAmount> {

        using deque = deque<T2, segmentSize, incStrat, IncAmount>;
        using Segment = typename deque::Segment;

        void ReallocateShift() {
            size_t old_cap = deque::segment_capacity_;
            deque::template IncreaseHelper<incStrat>();
            Segment* new_segments = new typename deque::Segment[deque::segment_capacity_];
            memset(new_segments, 0, sizeof(Segment) * deque::segment_capacity_);

            memcpy(new_segments, deque::segments_, sizeof(Segment) * old_cap);

            delete[] deque::segments_;
            deque::segments_ = new_segments;
        }
    public:
        stack() : deque() {
            deque::first_segment_ = 0;
            deque::last_segment_ = 0;
        }


        void push(const T2& _in) {
            deque::push_back(_in);
        }

        template<typename... Args>
        void emplace(Args&&... _args) {
            deque::emplace_back(static_cast<Args&&>(_args)...);
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

        size_t size() const {
            return deque::size();
        }

        /*
        void Dump() {
            deque::DumpSegment();
        }*/
    };
}

#endif //LAMPSTACK_H

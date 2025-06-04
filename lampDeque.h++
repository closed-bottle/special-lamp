//
// Created by JJ on 5/24/2025.
//

#ifndef LAMPDEQUE_H
#define LAMPDEQUE_H

#include "lampAssert.h++"

namespace Lamp
{
    enum SegmentIncrement {
        SegmentIncAdd = 0,
        SegmentIncDouble,
        SegmentIncCount
    };
    // Segment size should be fixed.
    template<typename T2, size_t segmentSize = 4, SegmentIncrement incStrat = SegmentIncDouble, size_t IncAmount = 16>
    class deque {
        struct Segment {
            size_t start_ = 0;
            size_t count_ = 0;
            T2 data_[segmentSize];

            inline bool IsFull() const {
                return count_ == segmentSize;
            }

            inline bool IsEmpty() const {
                return count_ == 0;
            }

            inline size_t LastIndex() const {
                return (start_ + count_) % segmentSize;
            }
        };
        static const size_t last_index_ = segmentSize -1;
        static constexpr size_t INITIAL_SEGMENT_COUNT = 5;

        Segment* segments_ = nullptr;
        size_t segment_capacity_ = INITIAL_SEGMENT_COUNT;
        size_t total_count_ = 0;
        size_t first_segment_ = INITIAL_SEGMENT_COUNT / 2;
        size_t last_segment_ = INITIAL_SEGMENT_COUNT / 2; // not necessary, can be computed by first_segment + count

        template<SegmentIncrement>
        void IncreaseHelper() {
            LAMPASSERT(false, "Using increment helper function without specialization is prohibited.");
        }

        template<>
        void IncreaseHelper<SegmentIncDouble>() {segment_capacity_ *= 2;}

        template<>
        void IncreaseHelper<SegmentIncAdd>() {segment_capacity_ += IncAmount;}

        template<SegmentIncrement increment = SegmentIncDouble>
        void ReallocateShift() {
            size_t old_count = segment_capacity_;
            IncreaseHelper<increment>();
            Segment* new_segments = new Segment[segment_capacity_];

            size_t new_first = (segment_capacity_ - old_count) / 2;
            size_t new_last = new_first + old_count -1;

            if (first_segment_ > last_segment_) {
                memcpy(&new_segments[new_first], &segments_[first_segment_], (old_count - first_segment_) * sizeof(Segment));
                // Guaranteed to be more than 1
                memcpy(&new_segments[old_count - first_segment_], &segments_[0], (last_segment_ + 1) * sizeof(Segment));
            }
            else {
                memcpy(&new_segments[new_first], &segments_[first_segment_], old_count * sizeof(Segment));
            }

            first_segment_ = new_first;
            last_segment_ = new_last;

            delete[] segments_;
            segments_ = new_segments;
        }


        public:
        deque() {
            segments_ = new Segment[INITIAL_SEGMENT_COUNT];
        }

        deque(const size_t& _initial_segment_size) : segment_capacity_(_initial_segment_size) {
            segments_ = new Segment[_initial_segment_size];
        }

        ~deque() {
            delete[] segments_;
        }

        const size_t& size() const {
            return total_count_;
        }

        T2& front() {
            LAMPASSERT(total_count_ != 0, "No data available.");
            return segments_[first_segment_].data_[segments_[first_segment_].start_];
        }

        T2& back() {
            LAMPASSERT(total_count_ != 0, "No data available.");
            Segment& curr = segments_[last_segment_];
            return curr.data_[(curr.start_ + curr.count_ - 1) % segmentSize];
        }

        void push_back(const T2& _in) {
            if (segments_[last_segment_].IsFull()) {
                const auto new_last_ = (last_segment_ + 1) % segment_capacity_;
                if (first_segment_ == new_last_) {
                    ReallocateShift<incStrat>();
                }
                else
                    last_segment_ = new_last_;
            }

            Segment& curr_segment = segments_[last_segment_];
            curr_segment.data_[curr_segment.LastIndex()] = _in;
            ++curr_segment.count_;
            ++total_count_;
        }

        void push_front(const T2& _in) {
            if (segments_[first_segment_].IsFull()) {
                const auto new_front = (first_segment_ + segment_capacity_ -1) % segment_capacity_;
                if (last_segment_ == new_front) {
                    ReallocateShift<incStrat>();
                }
                else
                    first_segment_ = new_front;
            }

            Segment& curr_segment = segments_[first_segment_];

            curr_segment.start_ = (curr_segment.start_ + segmentSize - 1) % segmentSize;
            curr_segment.data_[curr_segment.start_] = _in;
            ++curr_segment.count_;
            ++total_count_;
        }

        void pop_back() {
            if (total_count_) {
                --segments_[last_segment_].count_;
                --total_count_;
                if (segments_[last_segment_].IsEmpty() && total_count_ > 0) {
                    last_segment_ = last_segment_ == 0 ? segment_capacity_ - 1 : last_segment_ - 1;
                }
            }
        }

        void pop_front() {
            if (total_count_) {
                --total_count_;
                --segments_[first_segment_].count_;
                if (segments_[first_segment_].IsEmpty() && total_count_ > 0) {
                    first_segment_ = (first_segment_ + 1) % segment_capacity_;
                }
                else {
                    segments_[first_segment_].start_ = (segments_[first_segment_].start_ + 1) % segmentSize;
                }
            }
        }

        T2& operator[](size_t _index) {
            LAMPASSERT(_index < total_count_, "Index out of bound");

            // it is impossible to have empty first segment.
            size_t i = first_segment_ + (_index + (segmentSize - segments_[first_segment_].count_)) / segmentSize;
            i %= segment_capacity_;
            size_t j = 0;

            //[0,] , [1,2,3], index = 2
            // i = 1, j = 1;

            j = _index;
            if (i != first_segment_) {
                j -= segments_[first_segment_].count_;
                j += segments_[i].start_;
                j %= segmentSize;
            }
            else {
                j += segments_[first_segment_].start_;
                j %= segmentSize;
            }

            LAMPASSERT(i < segment_capacity_ && j < segmentSize, "Index out of bound");
            return segments_[i].data_[j];
        }

        void DumpSegment() {
            std::cout << "=============================" << std::endl;
            size_t i = first_segment_;
            while (true) {
                std::cout << "[";
                const Segment& curr = segments_[i];

                for (size_t j = 0; j < curr.count_; ++j) {
                    std::cout << curr.data_[(curr.start_ + j) % segmentSize] << ", ";
                }

                std::cout << "] : " << "start : " << curr.start_ << ", count : " << curr.count_ << std::endl;

                if (i == last_segment_)
                    break;
                i = (i + 1) % segment_capacity_;
            }
            std::cout << "=============================" << std::endl;
        }
    };
}

#endif //LAMPDEQUE_H

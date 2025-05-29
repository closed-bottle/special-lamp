//
// Created by JJ on 5/24/2025.
//

#ifndef LAMPSEGMENTEDTREE_H
#define LAMPSEGMENTEDTREE_H

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
    class SegmentedArray {
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
        size_t segment_count_ = INITIAL_SEGMENT_COUNT;
        size_t total_count_ = 0;
        size_t first_segment_ = INITIAL_SEGMENT_COUNT / 2;
        size_t last_segment_ = INITIAL_SEGMENT_COUNT / 2;

        template<SegmentIncrement increment>
        void ReallocateShift() {
            LAMPASSERT(false, "ReallocateShift without specialization is prohibited.");
        }

        template<>
        void ReallocateShift<SegmentIncDouble>() {
            size_t old_count = segment_count_;
            segment_count_ *= 2;
            Segment* new_segments = new Segment[segment_count_];

            size_t new_first = (segment_count_ - old_count) / 2;
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

        template<>
        void ReallocateShift<SegmentIncAdd>() {
            size_t old_count = segment_count_;
            segment_count_ += IncAmount;
            LAMPASSERT(false, "Not implemented yet");
            Segment* new_segments = new Segment[segment_count_];
        }



        public:
        SegmentedArray() {
            segments_ = new Segment[INITIAL_SEGMENT_COUNT];
        }

        SegmentedArray(const size_t& _initial_segment_size) : segment_count_(_initial_segment_size) {
            segments_ = new Segment[_initial_segment_size];
        }

        ~SegmentedArray() {
            delete[] segments_;
        }

        const size_t& TotalCount() const {
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
                const auto new_last_ = (last_segment_ + 1) % segment_count_;
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
                const auto new_front = (first_segment_ - 1) + (first_segment_ == 0 ? segment_count_ : 0);
                if (last_segment_ == new_front) {
                    ReallocateShift<incStrat>();
                }
                else
                    first_segment_ = new_front;
            }

            Segment& curr_segment = segments_[first_segment_];

            curr_segment.start_ = (curr_segment.start_ - 1) + (curr_segment.start_ == 0 ? segmentSize : 0);

            curr_segment.data_[curr_segment.start_] = _in;

            ++curr_segment.count_;
            ++total_count_;
        }

        void pop_back() {
            if (total_count_) {
                --segments_[last_segment_].count_;
                --total_count_;
                if (segments_[last_segment_].IsEmpty() && total_count_ > 0) {
                    last_segment_ = last_segment_ == 0 ? segment_count_ - 1 : last_segment_ - 1;
                }
            }
        }

        void pop_front() {
            if (total_count_) {
                --total_count_;
                --segments_[first_segment_].count_;
                if (segments_[first_segment_].IsEmpty() && total_count_ > 0) {
                    first_segment_ = (first_segment_ + 1) % segment_count_;
                }
                else {
                    segments_[first_segment_].start_ = (segments_[first_segment_].start_ + 1) % segmentSize;
                }
            }
        }

        T2& operator[](const size_t _index) {
            // Need to shift by first segment if it is not full but also not empty.
            size_t i = (first_segment_ + (_index + (segmentSize - segments_[first_segment_].count_)) / segmentSize)
                        % segment_count_;
            size_t j = ((_index % segmentSize) + segments_[first_segment_].count_) % segmentSize;

            return segments_[i].data_[j];
        }
    };
}

#endif //LAMPSEGMENTEDTREE_H

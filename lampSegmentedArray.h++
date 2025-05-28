//
// Created by JJ on 5/24/2025.
//

#ifndef LAMPSEGMENTEDTREE_H
#define LAMPSEGMENTEDTREE_H

#include "lampVector.h++"

namespace Lamp
{
    enum SegmentIncrement {
        SegmentIncAdd = 0,
        SegmentIncDouble,
        SegmentIncCount
    };
    // Segment size should be fixed.
    template<typename T2, size_t segmentSize = 4, SegmentIncrement = SegmentIncDouble, size_t IncAmount = 16>
    class SegmentedArray {
        struct Segment {
            size_t start_ = 0;
            size_t count_ = 0;
            T2 data_[segmentSize];

        public:
            inline bool IsFull() const {
                return count_ == segmentSize;
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
        bool isFull_ = true; // start without any initial segments.

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
            // Assert if empty
            return segments_[first_segment_].data_[segments_[first_segment_].start_];
        }

        T2& back() {
            // Assert if empty
            Segment& curr = segments_[last_segment_];
            return curr.data_[(curr.start_ + curr.count_ - 1) % segmentSize];
        }

        void push_back(const T2& _in) {
            if (segments_[last_segment_].IsFull()) {
                const auto new_last_ = (last_segment_ + 1) % segment_count_;
                if (first_segment_ == new_last_) {
                    // reallocate and shift
                }
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
                    // reallocate and shift
                }
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
            }
        }

        void pop_front() {
            if (total_count_) {
                ++segments_[first_segment_].start_;
                --segments_[first_segment_].count_;
                --total_count_;
            }
        }

        T2& operator[](const size_t _index) {
            size_t block_index = first_segment_ + (_index / segmentSize);
            size_t element_index = (segments_[block_index].start_ + _index) % segmentSize;

            return segments_[block_index].data_[element_index];
        }
    };
}

#endif //LAMPSEGMENTEDTREE_H

//
// Created by JJ on 6/6/2025.
//

#ifndef LAMPALLOCATOR_H
#define LAMPALLOCATOR_H

#include <cstdlib>

#include "lampString.h++"
#include "lampCRC32.h++"

namespace allocator {

namespace {
    template<typename T1, typename T2>
        struct pair {
        pair() = delete;

        pair(const T1 &t1, const T2 &t2) : first(t1), second(t2) {
        }

        bool operator==(const pair &_rhs) const {
            return _rhs.first == first && _rhs.second == second;
        }

        bool operator!=(const pair &_rhs) const {
            return _rhs.first != first || _rhs.second != second;
        }

        T1 first;
        T2 second;
    };


    template<typename T>
    class list {
        template<typename, typename>
        friend class allocated_unordered_map;

        struct node {
            template<typename, typename>
            friend class allocated_unordered_map;

            T data_;
            node *next_ = nullptr;

            node *next() {
                return next_;
            }
        };

        node *head_ = nullptr;
        node *tail_ = nullptr;
        uint64_t count_ = 0;

    public:
        list() : head_(nullptr), tail_(nullptr), count_(0) {
        }

        ~list() {
            while (!empty()) {
                node *new_head = head_->next_;
                free(head_);
                head_ = new_head;
                --count_;
            }
        }

        bool empty() const {
            return count_ == 0;
        }

        T &front() {
            return head_->data_;
        }

        T &back() {
            return tail_->data_;
        }

        const T &front() const {
            return head_->data_;
        }

        const T &back() const {
            return tail_->data_;
        }

        void push_front(const T &_value) {
            node *new_node = malloc(sizeof(node));
            new_node->data_ = _value;
            new_node->next_ = head_;

            head_ = new_node;

            if (count_ == 0) {
                tail_ = head_;
            }

            ++count_;
        }

        void push_back(const T &_value) {
            node *new_node = (node*)malloc(sizeof(node));
            new_node = new (new_node) node(_value, nullptr);

            if (tail_) {
                tail_->next_ = new_node;
            }

            tail_ = new_node;

            if (count_ == 0) {
                head_ = tail_;
            }

            ++count_;
        }

        T *at(uint64_t _pos) {
            node *n = head_;

            while (n && _pos) {
                n = n->next_;
                --_pos;
            }

            return &n->data_;
        }

        // _start == _end will erase single element.
        // Implementing this way so it's not confusing in case T == uint64
        void erase(uint64_t _start, uint64_t _size) {
            // Do not handle _start + _count > count_.
            if (_start == 0 && count_) {
                if (head_ == tail_) {
                    tail_ = nullptr;
                }

                while (_size) {
                    node *n = head_->next_;

                    free(head_);
                    head_ = n;
                    --_size;
                    --count_;
                }

                return;
            }

            node *prev = head_;

            while (prev && (_start - 1)) {
                prev = prev->next_;
                --_start;
            }


            node *curr = prev->next_;
            while (_size) {
                node *n = curr->next_;

                free(curr);
                curr = n;

                --_size;
                --count_;
            }
            prev->next_ = curr;

            if (!prev->next_) {
                tail_ = prev;
            }
        }

        void erase(const T &_value) {
            node *curr = head_;
            node *prev = nullptr;
            while (curr && curr->data_ != _value) {
                prev = curr;
                curr = curr->next_;
            }

            if (curr == head_) {
                if (head_ == tail_) {
                    tail_ = nullptr; // count_ == 0 edge case.
                }
                head_ = head_->next_;
                --count_;
                free(curr);
            } else if (curr == tail_) {
                if (head_ == tail_) {
                    head_ = nullptr;
                }
                tail_ = prev;
                --count_;
                free(curr);
            } else if (curr != nullptr) {
                prev->next_ = curr->next_;
                --count_;
                free(curr);
            }
        }

        void pop_front() {
            if (!head_) {
                // Ignore pop_front if list is empty.
                return;
            }

            node *new_head = head_->next_;
            free(head_);
            head_ = new_head;
            --count_;

            if (count_ == 1) {
                tail_ = head_;
            } else if (count_ == 0) {
                tail_ = nullptr;
            }
        }

        void pop_back() {
            if (!head_) {
                // Ignore pop_back if list is empty
                return;
            }
            node *prev = head_;

            while (prev && prev->next_ != tail_) {
                prev = prev->next_;
            }

            free(tail_);
            tail_ = prev;
            if (tail_) {
                tail_->next_ = nullptr;
            }
            --count_;

            if (count_ == 1) {
                head_ = tail_;
            } else if (count_ == 0) {
                head_ = nullptr;
            }
        }

        void insert(uint64_t _pos, const T &_data) {
            if (_pos == 0) {
                node *new_node = (node*)malloc(sizeof(node));
                new_node->data_ = _data;
                new_node->next_ = head_;

                head_ = new_node;
                tail_ = new_node;
                ++count_;
                return;
            } else if (_pos == count_) {
                push_back(_data);
                return;
            } else if (_pos > count_) {
                // Return if pos is out of bound instead of creating empty elements.
                return;
            }

            node *prev = head_;
            uint64_t curr = 0;
            while (prev && curr < _pos - 1) {
                prev = prev->next_;
                curr++;
            }

            node *new_node = (node*)malloc(sizeof(node));
            new_node->data_ = _data;
            new_node->next_ = prev->next_;

            prev->next_ = new_node;


            ++count_;
        }

        T *find(const T &_rhs, bool (*is_same)(const T &, const T &)) {
            node *curr = head_;
            while (curr) {
                if (is_same(curr, _rhs)) {
                    return curr->data_;
                }
                curr = curr->next_;
            }

            return nullptr;
        }
    };
}




namespace {
    template<typename T>
    uint32_t hash(const Lamp::CRC32 &_hash, const T &_key, const size_t _size) {
        return _hash.GetCRC32(reinterpret_cast<const uint8_t *>(&_key), _size);
    }
};

namespace {
    template<typename T1, typename T2>
    class allocated_unordered_map {
        list<pair<T1, T2>> *bucket_ = nullptr;
        uint64_t capacity_ = 8;
        uint64_t size_ = 0;
        float max_load_ = 0.5f;


        Lamp::CRC32 hash_function_;

        using node = list<pair<T1, T2> >::node;

        void RehashIfNeeded();

    public:
        uint64_t allocated_ = 0;

        allocated_unordered_map() {
            bucket_ = (list<pair<T1, T2>>*)malloc(sizeof(list<pair<T1, T2>>) * 8);
            memset(bucket_, 0, sizeof(list<pair<T1, T2>>) * 8);
            capacity_ = 8;
            size_ = 0;
            max_load_ = 0.5f;
        }

        ~allocated_unordered_map() {

            if (allocated_ != 0) {
                std::cout << allocated_ << "Memory leak detected";

                for (const auto& p : *this) {
                    //if (p.second == true)
                    //    std::cout << p.first << " : " << p.second << "bytes" << std::endl;
                }
            }


            free(bucket_);
        }

        void clear() {
            free(bucket_);
            bucket_ = (list<pair<void *, bool>> *)malloc(sizeof(list<pair<T1, T2>>) * 8);
            memset(bucket_, 0, sizeof(list<pair<T1, T2>>) * 8);

            capacity_ = 8;
            size_ = 0;
            max_load_ = 0.5f;
        }

        class iterator {
            friend iterator;
            friend allocated_unordered_map;
            allocated_unordered_map<T1, T2> *map_ = nullptr;
            uint64_t index_ = 0;
            pair<T1, T2> *data_ = nullptr;

        public:
            iterator() = delete;

            iterator(allocated_unordered_map<T1, T2> *_map, uint64_t _index, pair<T1, T2> *_data)
                : map_(_map), index_(_index), data_(_data) {
            }


            bool operator!=(const iterator &_rhs) const {
                return index_ != _rhs.index_ || data_ != _rhs.data_;
            }

            iterator operator++() {
                node *n = map_->bucket_[index_].head_;
                while (&n->data_ != data_) {
                    n = n->next_;
                }

                // If n is nullptr, then it must be application error.
                if (n->next_) {
                    data_ = &(n->next_->data_);
                    return *this;
                }

                ++index_;
                while (index_ < map_->capacity_) {
                    if (map_->bucket_[index_].empty()) {
                        ++index_;
                        continue;
                    }

                    n = map_->bucket_[index_].head_;
                    data_ = &(n->data_);
                    break;
                }

                if (index_ == map_->capacity_) {
                    data_ = nullptr;
                }

                return *this;
            }

            pair<T1, T2> operator*() {
                return *data_;
            }
        };

        iterator begin() {
            for (uint64_t i = 0; i < capacity_; ++i) {
                if (!bucket_[i].empty()) {
                    return {this, i, &bucket_[i].head_->data_};
                }
            }

            return {this, capacity_, nullptr};
        }

        iterator end() {
            return {this, capacity_, nullptr};
        }

        const iterator cbegin() const {
            for (uint64_t i = 0; i < capacity_; ++i) {
                if (!bucket_[i].empty()) {
                    return {this, i, &bucket_[i].head_->data_};
                }
            }

            return {this, capacity_, nullptr};
        }

        const pair<T1, T2> *cend() const {
            return {this, capacity_, nullptr};
        }

        const pair<T1, T2> *begin() const {
            for (uint64_t i = 0; i < capacity_; ++i) {
                if (!bucket_[i].empty()) {
                    return {this, i, &bucket_[i].head_->data_};
                }
            }

            return {this, capacity_, nullptr};
        }

        const pair<T1, T2> *end() const {
            return {this, capacity_, nullptr};
        }

        uint64_t size() const {
            return size_;
        }

        T2 *find(const T1 &_key) {
            list<pair<T1, T2> > &curr = bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_];
            node *curr_n = curr.head_;

            while (curr_n) {
                if (curr_n->data_.first == _key) {
                    return &curr_n->data_.second;
                }
                curr_n = curr_n->next_;
            }

            return nullptr;
        }

        void insert(const T1 &_key, const T2 &_value) {
            T2 *found = find(_key);

            if (found) {
                *found = _value;
                return;
            }

            bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_].push_back({_key, _value});
            ++size_;
            RehashIfNeeded();
        }

        void erase(const T1 &_key) {
            list<pair<T1, T2> > &curr = bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_];
            node *curr_n = curr.head_;

            while (curr_n) {
                if (curr_n->data_.first == _key) {
                    curr.erase(curr_n->data_);
                    --size_;
                    return;
                }
                curr_n = curr_n->next_;
            }
        }

        T2 &operator[](const T1 &_key) {
            list<pair<T1, T2>> &curr = bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_];

            if (curr.empty()) {
                // insert
                insert(_key, {});

                return *find(_key);
            }
            node * prev2 = nullptr;
            node *prev = nullptr;
            node *curr_n = curr.head_;

            while (curr_n) {
                if (curr_n->data_.first == _key) {
                    return curr_n->data_.second;
                }
                prev2 = prev;
                prev = curr_n;
                curr_n = curr_n->next_;
            }

            insert(_key, {});

            return *find(_key);
        }
    };

    template<typename T1, typename T2>
    void allocated_unordered_map<T1, T2>::RehashIfNeeded() {
        if (static_cast<float>(size_) / capacity_ <= max_load_) {
            return;
        }

        uint64_t new_cap = (capacity_ + 1) * 2;
        list<pair<T1, T2>> *new_bucket = (list<pair<T1, T2>> *)malloc(sizeof(list<pair<T1, T2>>) * (new_cap + 1));
        memset(new_bucket, 0, sizeof(list<pair<T1, T2>>) * (new_cap + 1));

        for (uint64_t i = 0; i < capacity_; ++i) {
            list<pair<T1, T2>>& curr = bucket_[i];
            node *curr_n = curr.head_;

            while (curr_n) {
                new_bucket[hash(hash_function_, curr_n->data_.first, sizeof(T1)) % new_cap].push_back(curr_n->data_);

                curr_n = curr_n->next_;
            }
        }

        free(bucket_);
        bucket_ = new_bucket;
        capacity_ = new_cap;
    }
}
}
static allocator::allocated_unordered_map<void*, bool> allocation_track;
static int debgcount;

void* operator new(size_t _size) {
    void* buff = malloc(_size);
    allocation_track[buff] = true;
    ++allocation_track.allocated_;

    return buff;
}

void* operator new[](size_t _size) {
    void* buff = malloc(_size);
    allocation_track[buff] = true;
    ++allocation_track.allocated_;

    return buff;
}

void operator delete(void* _ptr, size_t _size) noexcept {
    allocation_track[_ptr] = false;
    free(_ptr);

    --allocation_track.allocated_;
    if (allocation_track.allocated_ == 0)
        allocation_track.clear();
}

void operator delete[](void* _ptr) noexcept {
    allocation_track[_ptr] = false;
    free(_ptr);

    --allocation_track.allocated_;
    if (allocation_track.allocated_ == 0)
        allocation_track.clear();
}

#endif //LAMPALLOCATOR_H

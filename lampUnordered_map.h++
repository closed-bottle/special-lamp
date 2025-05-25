#ifndef LAMPUNORDERED_MAP_H
#define LAMPUNORDERED_MAP_H

#include "lampList.h++"
#include "lampPair.h++"
#include "lampString.h++"


#ifndef LAMP_UNORDERED_MAP_USE_MURMUR3
#define LAMP_UNORDERED_MAP_USE_CRC32
#endif

#ifdef LAMP_UNORDERED_MAP_USE_CRC32
#include "lampCRC32.h++"
#else
#ifdef LAMP_UNORDERED_MAP_USE_MURMUR3
#include "lampMurmur3.h++"
#endif
#endif

namespace {
#ifdef LAMP_UNORDERED_MAP_USE_CRC32
    template<typename T>
    uint32_t hash(const Lamp::CRC32 &_hash, const T &_key, const size_t _size) {
        return _hash.GetCRC32(reinterpret_cast<const uint8_t *>(&_key), _size);
    }

    template<>
    uint32_t hash(const Lamp::CRC32 &_hash, const Lamp::String &_key, const size_t _size) {
        return _hash.GetCRC32(reinterpret_cast<const uint8_t *>(_key.c_str()), _key.length() - 1); // Ignore null.
    }

#endif
};

namespace Lamp {
    template<typename T1, typename T2>
    class unordered_map {
        list<pair<T1, T2> > *bucket_ = new list<pair<T1, T2> >[8];
        uint64_t capacity_ = 8;
        uint64_t size_ = 0;
        float max_load_ = 0.5f;

#ifdef LAMP_UNORDERED_MAP_USE_CRC32
        CRC32 hash_function_;
#endif

        using node = list<pair<T1, T2> >::node;

        void RehashIfNeeded();

    public:
        ~unordered_map() {
            delete[] bucket_;
        }

        void clear() {
            delete[] bucket_;
            bucket_ = new list<pair<T1, T2> >[8];
            capacity_ = 8;
            size_ = 0;
            max_load_ = 0.5f;
        }

        class iterator {
            friend iterator;
            friend unordered_map;
            unordered_map<T1, T2> *map_ = nullptr;
            uint64_t index_ = 0;
            pair<T1, T2> *data_ = nullptr;

        public:
            iterator() = delete;

            iterator(unordered_map<T1, T2> *_map, uint64_t _index, pair<T1, T2> *_data)
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
            uint64_t i = 0;

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
            uint64_t i = 0;

            while (curr_n) {
                if (curr_n->data_.first == _key) {
                    curr.erase(curr_n->data_);
                    return;
                }
                curr_n = curr_n->next_;
            }
        }

        T2 &operator[](const T1 &_key) {
            list<pair<T1, T2> > &curr = bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_];

            if (curr.empty()) {
                // insert
                insert(_key, {});

                return *find(_key);
            }
            node *curr_n = curr.head_;

            while (curr_n) {
                if (curr_n->data_.first == _key) {
                    return curr_n->data_.second;
                }
                curr_n = curr_n->next_;
            }

            insert(_key, {});

            return *find(_key);
        }
    };

    template<typename T1, typename T2>
    void unordered_map<T1, T2>::RehashIfNeeded() {
        if (static_cast<float>(size_) / capacity_ <= max_load_) {
            return;
        }

        uint64_t new_cap = (capacity_ + 1) * 2;
        list<pair<T1, T2> > *new_bucket = new list<pair<T1, T2> >[new_cap + 1];

        memset(new_bucket, 0, sizeof(list<pair<T1, T2> >) * new_cap);

        for (uint64_t i = 0; i < capacity_; ++i) {
            list<pair<T1, T2> > &curr = bucket_[i];
            node *curr_n = curr.head_;

            while (curr_n) {
                new_bucket[hash(hash_function_, curr_n->data_.first, sizeof(T1)) % new_cap].push_back(curr_n->data_);

                curr_n = curr_n->next_;
            }
        }

        delete[] bucket_;
        bucket_ = new_bucket;
        capacity_ = new_cap;
    }
}

#endif //LAMPUNORDERED_MAP_H

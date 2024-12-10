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
    uint32_t hash(const Lamp::CRC32 &_hash, const T & _key, const size_t _size) {
        return _hash.GetCRC32(_key, _size);
    }

    template<>
    uint32_t hash(const Lamp::CRC32 &_hash, const Lamp::String & _key, const size_t _size) {
        return _hash.GetCRC32(reinterpret_cast<const uint8_t *>(_key.c_str()), _key.length() - 1); // Ignore null.
    }

#endif

};

namespace Lamp {
    template <typename T1, typename T2>
    class unordered_map {
        list<pair<T1, T2>> * bucket_ = new list<pair<T1, T2>>[8];
        uint64_t capacity_ = 8;
        uint64_t size_ = 0;
        float max_load_ = 0.5f;

#ifdef LAMP_UNORDERED_MAP_USE_CRC32
        CRC32 hash_function_;
#endif

        using node = list<pair<T1, T2>>::node;

        void RehashIfNeeded();

        public:
        ~unordered_map() {
            delete[] bucket_;
        }

        T2 * find(const T1 & _key) {
            list<pair<T1, T2>>& curr = bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_];
            node* curr_n = curr.head_;
            uint64_t i = 0;

            while (curr_n) {
                if (curr_n->data_.first == _key) {
                    return &curr_n->data_.second;
                }
                curr_n = curr_n->next_;
            }

            return nullptr;
        }

        void insert(const T1 & _key, const T2 & _value) {
            T2* found = find(_key);

            if (found) {
                *found = _value;
                return;
            }

            bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_].push_back({_key, _value});
            ++size_;
            RehashIfNeeded();
        }

        void erase(const T1 & _key) {
            list<pair<T1, T2>>& curr = bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_];
            node* curr_n = curr.head_;
            uint64_t i = 0;

            while (curr_n) {
                if (curr_n->data_.first == _key) {
                    curr.erase(curr_n->data_);
                    return;
                }
                curr_n = curr_n->next_;
            }
        }

        T2 & operator[](const T1 & _key) {
            list<pair<T1, T2>> & curr = bucket_[hash(hash_function_, _key, sizeof(T1)) % capacity_];

            if (curr.empty()) {
                // insert
                insert(_key, {});

                return *find(_key);
            }
            node* curr_n = curr.head_;

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
        list<pair<T1, T2>> * new_bucket = new list<pair<T1, T2>>[new_cap];

        memset(new_bucket, 0, sizeof(list<pair<T1, T2>>) * new_cap);

        for (uint64_t i = 0; i < capacity_; ++i) {

            list<pair<T1, T2>> & curr = bucket_[i];
            node* curr_n = curr.head_;

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

#ifndef LAMPVECTOR_H
#define LAMPVECTOR_H
#include <cstdint>
#include <cstring>

#include <iostream>

namespace Lamp {

template <typename T>
class Vector {
        uint64_t size_ = 0;
        uint64_t capacity_ = 0;
        T* data_ = nullptr;
    public:
        Vector() = default;
        Vector(const uint64_t& _size, const T& _value) {
            size_ = _size;
            capacity_ = _size;
            data_ = new T[size_];

            for (uint64_t i = 0; i < size_; i++) {
                data_[i] = _value;
            }
        }

        ~Vector() {
            delete[] data_;
        }

        Vector(const Vector &_rhs) {
            size_ = _rhs.size_;
            capacity_ = _rhs.capacity_;
            data_ = new T[size_];
            memcpy(data_, _rhs.data_, size_ * sizeof(T));
        }

        Vector & operator=(const Vector & _rhs) {
            if (size_ == _rhs.size_ && capacity_ == _rhs.capacity_ && data_ == _rhs.data_) {
                return *this;
            }

            size_ = _rhs.size_;

            if (capacity_ < _rhs.capacity_) {
                delete[] data_;

                data_ = new T[size_];
            }

            capacity_ = _rhs.capacity_;

            memcpy(data_, _rhs.data_, size_ * sizeof(T));
            return *this;
        }

        bool operator==(const Vector & _rhs) const {
            if (size_ != _rhs.size_ || capacity_ != _rhs.capacity_) {
                return false;
            }
            return true;
        }

        T& operator[](const uint64_t& _index) {
            return data_[_index];
        }

        uint64_t size() const {
            return size_;
        }

        void push_back(T const& _value) {
            if (capacity_ == size_) {
                capacity_ = (capacity_ + 1) * 2;

                T* new_data = new T[capacity_];
                memcpy(new_data, data_, size_ * sizeof(T));
                data_ = new_data;
            }

            data_[size_] = _value;
            size_++;
        }

        void push_back(T && _value) {

            if (capacity_ == size_) {
                capacity_ = (capacity_ + 1) * 2;

                T* new_data = new T[capacity_];
                memcpy(new_data, data_, size_ * sizeof(T));
                data_ = new_data;
            }

            data_[size_] = std::move(_value);
            size_++;
        }

        void pop_back() {
            size_--;
        }

        T* data() const {
            return data_;
        }

        bool if_contain(Vector<T> const& _rhs) {
            if (*this == _rhs) {
                return true;
            }
            else if (size_ < _rhs.size_) {
                return false;
            }

            uint64_t count = 0;

            for (uint64_t i = 0; i < size_; i++) {
                for (uint64_t j = 0; j < _rhs.size_; j++) {
                    if (data_[i] == _rhs.data_[j]) {
                        ++count;
                    }
                }
            }

            return count == _rhs.size_;
        }

    bool if_contain(Vector<T> const& _rhs, bool(*_if_same)(T const &, T const &)) {
            if (*this == _rhs) {
                return true;
            }
            else if (size_ < _rhs.size_) {
                return false;
            }

            uint64_t count = 0;

            for (uint64_t i = 0; i < size_; i++) {
                for (uint64_t j = 0; j < _rhs.size_; j++) {
                    if (_if_same(data_[i], _rhs.data_[j])) {
                        ++count;
                    }
                }
            }

            return count == _rhs.size_;
        }

    void reserve(uint64_t _size) {
            if (capacity_ > _size) {
                if (size_ < _size) {
                    size_ = _size;
                }
            }
            else {
                capacity_ = _size;
                size_ = _size;

                T* new_data = new T[capacity_];
                memcpy(new_data, data_, size_ * sizeof(T));

                delete[] data_;
                data_ = new_data;
            }
        }
};
}
#endif //LAMPVECTOR_H

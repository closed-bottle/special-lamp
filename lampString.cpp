
#include "lampString.h"

#include <string.h>

Lamp::String::String() {
    data_ = nullptr;
    length_ = 0;
    capacity_ = 0;
}

Lamp::String::String(const char * _cstr) {
    length_ = 0;
    while (_cstr[length_] != '\0') {
        ++length_;
    }
    ++length_; // for 'null' char.
    capacity_ = length_;
    data_ = new char[capacity_];

    for (int i = 0; i < length_; ++i) {
        data_[i] = _cstr[i];
    }
}

Lamp::String::String(const String & _rhs) :length_(_rhs.length_), capacity_(_rhs.length_)
{
    data_ = new char[capacity_];
    memcpy(data_, _rhs.data_, length_);
}

Lamp::String::~String() {
    if (data_ != nullptr) {
        delete[] data_;
    }

    length_ = 0;
    capacity_ = 0;
}

inline Lamp::String & Lamp::String::operator=(const String & _rhs) {
    if (capacity_ < _rhs.length_) {
        capacity_ = _rhs.length_;
        delete[] data_;
        data_ = new char[capacity_];
    }

    length_ = _rhs.length_;

    memcpy(data_, _rhs.data_, length_);

    return *this;
}

inline Lamp::String & Lamp::String::operator=(const char * _rhs) {
    uint32_t count = 0;
    while (_rhs[count] != '\0') {
        ++count;
    }
    ++count; // include null

    if (capacity_ < count) {
        capacity_ = count;
        delete[] data_;
        data_ = new char[capacity_];
    }

    memcpy(data_, _rhs, count);

    return *this;
}

inline bool Lamp::String::operator==(const String & _rhs) const {
    if (length_ != _rhs.length_) {
        return false;
    }

    for (int i = 0; i < length_; ++i) {
        if (data_[i] != _rhs.data_[i])
            return false;
    }

    return true;
}

inline bool Lamp::String::operator!=(const String & _rhs) const {
    return !(*this == _rhs);
}

inline bool Lamp::String::operator==(const char * _rhs) const {
    uint32_t i = 0;

    while (_rhs[i] != '\0') {
        if (data_[i] != _rhs[i])
            return false;
    }

    return true;
}

inline bool Lamp::String::operator!=(const char * _rhs) const {
    return !(*this == _rhs);
}

inline char & Lamp::String::operator[](const uint32_t & _rhs) const {
    return data_[_rhs];
}

const char * Lamp::String::c_str() const {
    return data_;
}

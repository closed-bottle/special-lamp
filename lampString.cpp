
#include "lampString.h"
#include <cstring>
#include <iostream>

using namespace Lamp;

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

    memcpy(data_, _cstr, length_);
}

Lamp::String::String(String const & _rhs) :length_(_rhs.length_), capacity_(_rhs.length_)
{
    data_ = new char[capacity_];
    memcpy(data_, _rhs.data_, length_);
}

String::String(String && _rhs) noexcept  :length_(_rhs.length_), capacity_(_rhs.length_) {
    data_ = _rhs.data_;
    _rhs.data_ = nullptr;
}

Lamp::String::~String() {
    delete[] data_;
    length_ = 0;
    capacity_ = 0;
}

Lamp::String & Lamp::String::operator=(Lamp::String const &_rhs) {
    if (capacity_ < _rhs.length_) {
        capacity_ = _rhs.length_;
        delete[] data_;
        data_ = new char[capacity_];
    }

    length_ = _rhs.length_;

    memcpy(data_, _rhs.data_, length_);

    return *this;
}

String & String::operator=(String && _rhs) {
    delete[] data_;
    length_ = _rhs.length_;
    capacity_ = _rhs.capacity_;
    data_ = _rhs.data_;
    _rhs.data_ = nullptr;
    _rhs.capacity_ = 0;
    _rhs.length_ = 0;


    return *this;
}

Lamp::String & Lamp::String::operator=(const char * _rhs) {
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

bool Lamp::String::operator==(String const & _rhs) const {
    if (length_ != _rhs.length_) {
        return false;
    }

    for (int i = 0; i < length_; ++i) {
        if (data_[i] != _rhs.data_[i])
            return false;
    }

    return true;
}

bool Lamp::String::operator==(const char * _rhs) const {
    uint32_t i = 0;

    while (_rhs[i] != '\0') {
        if (data_[i] != _rhs[i])
            return false;
        ++i;
    }

    return true;
}
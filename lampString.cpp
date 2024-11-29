
#include "lampString.h"

#include <string.h>

Lamp::String::String() {
    data = nullptr;
    length = 0;
    capacity = 0;
}

Lamp::String::String(const char * _cstr) {
    length = 0;
    while (_cstr[length] != '\0') {
        ++length;
    }
    capacity = length;
    data = new char[capacity];

    for (int i = 0; i < length; ++i) {
        data[i] = _cstr[i];
    }
}

Lamp::String::String(const String & _rhs) :length(_rhs.length), capacity(_rhs.length)
{
    data = new char[capacity];
    memcpy(data, _rhs.data, length);
}

Lamp::String::~String() {
    if (data != nullptr) {
        delete[] data;
    }

    length = 0;
    capacity = 0;
}

Lamp::String & Lamp::String::operator=(const String & _rhs) {
    if (capacity < _rhs.length) {
        capacity = _rhs.length;
        delete[] data;
        data = new char[capacity];
    }

    length = _rhs.length;

    memcpy(data, _rhs.data, length);

    return *this;
}

Lamp::String & Lamp::String::operator=(const char * _rhs) {
    uint32_t count = 0;
    while (_rhs[count] != '\0') {
        ++count;
    }

    if (capacity < count) {
        capacity = count;
        delete[] data;
        data = new char[capacity];
    }

    memcpy(data, _rhs, count);

    return *this;
}

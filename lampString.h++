
#ifndef LAMP_STRING_H
#define LAMP_STRING_H

#include <iostream>
#include <cstdint>

namespace Lamp {

namespace {
    template <typename T>
    class String_t {
    private:
        T* data_;
        uint32_t length_;
        uint32_t capacity_;

    public:

        String_t();
        String_t(const char*);
        String_t(String_t const &);
        String_t(const char *, uint32_t);
        String_t(String_t&&) noexcept;
        ~String_t();

        String_t&& operator+(String_t const & _rhs);
        String_t& operator=(String_t const &);
        String_t& operator=(String_t&&);
        String_t& operator=(const char*);

        bool operator==(String_t const &) const;
        inline bool operator!=(String_t const & _rhs) const {
            return !(*this == _rhs);
        }
        bool operator==(const char*) const;
        inline bool operator!=(const char* _rhs) const {
            return !(*this == _rhs);
        }


        inline char& operator[](uint32_t const & _rhs) const {
            return data_[_rhs];
        }

        inline const char * c_str() const {
            return data_;
        }

        inline uint32_t length() const {
            return length_; // Include null.
        }
    };

    template <typename T>
    Lamp::String_t<T>::String_t() {
        data_ = nullptr;
        length_ = 0;
        capacity_ = 0;
    }

    template <typename T>
    Lamp::String_t<T>::String_t(const char * _cstr) {
        length_ = 0;
        while (_cstr[length_] != '\0') {
            ++length_;
        }
        ++length_; // for 'null' char.
        capacity_ = length_;
        data_ = new char[capacity_];

        memcpy(data_, _cstr, length_);
    }

    template <typename T>
    Lamp::String_t<T>::String_t(String_t const & _rhs) :length_(_rhs.length_), capacity_(_rhs.length_)
    {
        data_ = new char[capacity_];
        memcpy(data_, _rhs.data_, length_);
    }

    template<typename T>
    String_t<T>::String_t(const char * _data, uint32_t _size) {
        length_ = _size;
        capacity_ = _size;
        data_ = new char[capacity_];

        memcpy(data_, _data, length_);
    }

    template <typename T>
    String_t<T>::String_t(String_t && _rhs) noexcept  :length_(_rhs.length_), capacity_(_rhs.length_) {
        data_ = _rhs.data_;
        _rhs.data_ = nullptr;
    }

    template <typename T>
    Lamp::String_t<T>::~String_t() {
        delete[] data_;
        length_ = 0;
        capacity_ = 0;
    }

    template <typename T>
    Lamp::String_t<T>&& Lamp::String_t<T>::operator+(Lamp::String_t<T> const & _rhs) {
        String_t new_string;



        return std::move(new_string);
    }

    template <typename T>
    Lamp::String_t<T> & Lamp::String_t<T>::operator=(Lamp::String_t<T> const &_rhs) {
        if (capacity_ < _rhs.length_) {
            capacity_ = _rhs.length_;
            delete[] data_;
            data_ = new char[capacity_];
        }

        length_ = _rhs.length_;

        memcpy(data_, _rhs.data_, length_);

        return *this;
    }

    template <typename T>
    String_t<T> & String_t<T>::operator=(String_t<T> && _rhs) {
        delete[] data_;
        length_ = _rhs.length_;
        capacity_ = _rhs.capacity_;
        data_ = _rhs.data_;
        _rhs.data_ = nullptr;
        _rhs.capacity_ = 0;
        _rhs.length_ = 0;


        return *this;
    }

    template <typename T>
    Lamp::String_t<T> & Lamp::String_t<T>::operator=(const char * _rhs) {
        length_ = 0;
        while (_rhs[length_] != '\0') {
            ++length_;
        }
        ++length_; // include null

        if (capacity_ < length_) {
            capacity_ = length_;
            delete[] data_;
            data_ = new char[capacity_];
        }

        memcpy(data_, _rhs, length_);

        return *this;
    }

    template <typename T>
    bool Lamp::String_t<T>::operator==(String_t<T> const & _rhs) const {
        if (length_ != _rhs.length_) {
            return false;
        }

        for (int i = 0; i < length_; ++i) {
            if (data_[i] != _rhs.data_[i])
                return false;
        }

        return true;
    }

    template <typename T>
    bool Lamp::String_t<T>::operator==(const char * _rhs) const {
        uint32_t i = 0;

        while (_rhs[i] != '\0') {
            if (data_[i] != _rhs[i])
                return false;
            ++i;
        }

        return true;
    }
}

    using String = String_t<char>;
    using WString = String_t<wchar_t>;

}


#endif //LAMP_STRING_H

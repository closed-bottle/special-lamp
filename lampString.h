
#ifndef LAMP_STRING_H
#define LAMP_STRING_H

#include <cstdint>

namespace Lamp {

class String {
    private:
        char* data_;
        uint32_t length_;
        uint32_t capacity_;

    public:

        String();
        String(const char*);
        String(String const &);
        String(String&&) noexcept;
        ~String();

        String& operator=(String const &);
        String& operator=(String&&);
        String& operator=(const char*);

        bool operator==(String const &) const;
        inline bool operator!=(String const & _rhs) const {
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
};

}


#endif //LAMP_STRING_H

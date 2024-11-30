
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
        explicit String(const char*);
        String(const String&);
        ~String();

        String& operator=(const String&);
        String& operator=(const char*);
        bool operator==(const String&) const;
        bool operator!=(const String&) const;
        bool operator==(const char*) const;
        bool operator!=(const char*) const;
        char& operator[](const uint32_t&) const;
        [[nodiscard]] const char* c_str() const;
};

}


#endif //LAMP_STRING_H

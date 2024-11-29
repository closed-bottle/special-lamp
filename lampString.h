
#ifndef LAMP_STRING_H
#define LAMP_STRING_H

#include <cstdint>

namespace Lamp {

class String {
    private:
        char* data;
        uint32_t length;
        uint32_t capacity;

    public:

        String();
        explicit String(const char*);
        String(const String&);
        ~String();

        String& operator=(const String&);
        String& operator=(const char*);
        const char* c_str() const;
};

}


#endif //LAMP_STRING_H

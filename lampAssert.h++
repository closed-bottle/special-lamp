#ifndef LAMPASSERT_H
#define LAMPASSERT_H

#include <iostream>
#ifdef LAMP_DEBUG
#define LAMPASSERT(condition, msg, ...) do { \
if (!(condition)) { \
std::cerr << "ERROR : " << __FILE__ << ":" << __LINE__ << ", " << __func__ << std::endl; \
std::cerr << "MSG   : " << (msg) << std::endl; \
abort(); \
} \
} while(0)
#else
#define LAMPASSERT(condition, msg, ...) ((void)0)
#endif


#endif //LAMPASSERT_H

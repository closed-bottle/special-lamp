#ifndef LAMPUTILITY_H
#define LAMPUTILITY_H

#include <cstdint>

namespace Lamp {
  enum class Endianness { Invalid, Little, Big, Count };

  template <typename T>
  T Clamp(const T& _src, const T& _min, const T& _max) {
    T result = _src;

    if (result < _min) {
      result = _min;
    }
    else if (result > _max) {
      result = _max;
    }

    return result;
  }

  template <typename T>
  constexpr Endianness CheckEndianness() {
    T end = static_cast<T>(1);
    uint8_t endianness = reinterpret_cast<uint8_t>(reinterpret_cast<uint8_t*>(&end)[0]);

    T if_big_endian = 1 << ((sizeof(T) * 8) - 1);

    if (endianness == 1) {
      return Endianness::Little;
    }
    else if (if_big_endian == 1) {
      return Endianness::Big;
    }

    return Endianness::Invalid;
  }

  template <typename T>
  constexpr bool IsLittleEndian() {
    return CheckEndianness<T>() == Endianness::Little;
  }

  constexpr size_t PtrSize() { return sizeof(int*); }

  template <typename T>
  struct RemoveRef {
    typedef T type;
  };

  template <typename T>
  struct RemoveRef<T&> {
    typedef T type;
  };

  template <typename T>
  struct RemoveRef<T&&> {
    typedef T type;
  };

  template <typename T>
  struct RemovePtr {
    typedef T type;
  };

  template <typename T>
  struct RemovePtr<T*> {
    typedef T type;
  };

  template <typename T>
  constexpr typename RemoveRef<T>::type&& move(T&& _in) noexcept {
    return static_cast<typename RemoveRef<T>::type&&>(_in);
  }

  template <typename T>
  void Swap(T& _lhs, T& _rhs) {
    T temp = move(_lhs);
    _lhs = move(_rhs);
    _rhs = move(temp);
  }
} // namespace Lamp

#endif // LAMPUTILITY_H

#ifndef LAMPVECTOR_H
#define LAMPVECTOR_H
#include "lampAssert.h++"
#include "lampUtility.h++"

namespace Lamp {
  template <typename T>
  class Vector {
    uint64_t size_ = 0;
    uint64_t capacity_ = 0;
    T* data_ = nullptr;

    T* AllocData() { return new T[capacity_ + 1]; }

    void ReAllocData(const uint64_t _new_capacity) {
      capacity_ = _new_capacity;
      T* new_data = AllocData();

      for (uint64_t i = 0; i < size_; ++i) {
        new_data[i] = Lamp::move(data_[i]);
      }
      delete[] data_;
      data_ = new_data;
    }

    void ReAllocIfNeeded() {
      if (capacity_ == size_) {
        ReAllocData((capacity_ + 1) * 2);
      }
    }

  public:
    Vector() = default;

    Vector(const uint64_t& _size, const T& _value) {
      size_ = _size;
      capacity_ = _size;
      if (capacity_) {
        data_ = AllocData();
      }

      for (uint64_t i = 0; i < size_; i++) {
        data_[i] = _value;
      }
    }

    ~Vector() {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }

    Vector(const Vector& _rhs) {
      size_ = _rhs.size_;
      capacity_ = _rhs.capacity_;
      if (capacity_) {
        data_ = AllocData();
      }

      for (uint64_t i = 0; i < size_; ++i) {
        data_[i] = _rhs.data_[i];
      }
    }

    Vector(Vector&& _rhs) noexcept {
      size_ = _rhs.size_;
      capacity_ = _rhs.capacity_;
      data_ = _rhs.data_;


      _rhs.data_ = nullptr;
      _rhs.capacity_ = 0;
      _rhs.size_ = 0;
    }


    Vector(const uint8_t* _data_stream, const uint64_t _byte_size) {
      // Dangerous constructor...
      const uint16_t stride = sizeof(T);
      const uint64_t count = _byte_size / stride;

      if (_byte_size % stride) {
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
        return;
      }

      size_ = count;
      capacity_ = count;
      if (capacity_) {
        data_ = AllocData();
      }

      if (Lamp::CheckEndianness<T>() == Endianness::Little) {
        for (uint64_t i = 0; i < _byte_size; i += stride) {
          // First byte to LSB.
          T curr = 0;
          for (uint16_t j = 0; j < stride; ++j) {
            curr |= _data_stream[i + j] << j * 8;
          }

          data_[i / stride] = curr;
        }
      }
      else if (Lamp::CheckEndianness<T>() == Endianness::Big) {
        for (uint64_t i = 0; i < _byte_size; i += stride) {
          // First byte to MSB.
          T curr = 0;
          for (uint16_t j = 0; j < stride; ++j) {
            curr |= _data_stream[i + j] << (stride - 1 - j) * 8;
          }

          data_[i / stride] = curr;
        }
      }
    }


    Vector& operator=(const Vector& _rhs) {
      if (this == &_rhs)
        return *this;

      if (capacity_ < _rhs.size_) {
        delete[] data_;

        capacity_ = _rhs.size_;
        if (capacity_) {
          data_ = AllocData();
        }
      }

      size_ = _rhs.size_;

      for (uint64_t i = 0; i < size_; ++i) {
        data_[i] = _rhs.data_[i];
      }
      return *this;
    }

    Vector& operator=(Vector&& _rhs) noexcept {
      if (this == &_rhs)
        return *this;


      size_ = _rhs.size_;
      capacity_ = _rhs.capacity_;
      data_ = _rhs.data_;

      _rhs.data_ = nullptr;
      _rhs.capacity_ = 0;
      _rhs.size_ = 0;

      return *this;
    }

    T& operator[](const uint64_t& _index) { return data_[_index]; }

    T const& operator[](const uint64_t& _index) const { return data_[_index]; }

    T* begin() { return data_; }

    T* end() { return data_ + size_; }

    const T* cbegin() const { return data_; }

    const T* cend() const { return data_ + size_; }

    const T* begin() const { return data_; }

    const T* end() const { return data_ + size_; }

    [[nodiscard]] uint64_t size() const { return size_; }

    [[nodiscard]] uint64_t capacity() const { return capacity_; }

    [[nodiscard]] bool empty() const { return size_ == 0; }

    void shrink_to_fit() { ReAllocData(size_); }

    void push_back(T const& _value) {
      ReAllocIfNeeded();

      data_[size_] = _value;
      size_++;
    }

    void push_back(T&& _value) {
      ReAllocIfNeeded();

      data_[size_] = Lamp::move(_value);
      size_++;
    }

    template <typename... Args>
    void emplace_back(Args&&... _args) {
      ReAllocIfNeeded();

      new (data_ + size_) T(static_cast<Args&&>(_args)...);
      size_++;
    }

    void pop_back() {
      LAMPASSERT(size_ != 0, "Failed to pop_back. This vector is empty.");
      size_--;
    }

    T* data() const { return data_; }

    bool if_contain(Vector const& _rhs) const {
      if (*this == _rhs) {
        return true;
      }

      if (size_ < _rhs.size_) {
        return false;
      }

      uint64_t count = 0;

      for (uint64_t i = 0; i < size_; i++) {
        for (uint64_t j = 0; j < _rhs.size_; j++) {
          if (data_[i] == _rhs.data_[j]) {
            ++count;
            break;
          }
        }
      }

      return count == _rhs.size_;
    }

    bool if_contain(Vector const& _rhs, bool (*_if_same)(T const&, T const&)) const {
      if (size_ < _rhs.size_) {
        return false;
      }

      uint64_t count = 0;

      for (uint64_t i = 0; i < size_; i++) {
        for (uint64_t j = 0; j < _rhs.size_; j++) {
          if (_if_same(data_[i], _rhs.data_[j])) {
            ++count;
            break;
          }
        }
      }

      return count == _rhs.size_;
    }

    bool if_contain(const T& _rhs) const {
      for (uint64_t i = 0; i < size_; ++i) {
        if (data_[i] == _rhs) {
          return true;
        }
      }

      return false;
    }

    /* Reserve:
     * 1. Does not change the size of the vector.
     * 2. If new capacity is smaller than current, nothing should happen.
     */
    void reserve(const uint64_t _new_cap) {
      if (_new_cap == 0) {
        return;
      }

      if (capacity_ > _new_cap) {
        /*nothing*/
      }
      else {
        capacity_ = _new_cap;

        T* new_data = AllocData();
        if (data_) {
          for (uint64_t i = 0; i < size_; ++i) {
            new_data[i] = Lamp::move(data_[i]);
          }
        }

        delete[] data_;
        data_ = new_data;
      }
    }

    void resize(const uint64_t _new_size) {
      if (_new_size > capacity_) {
        ReAllocIfNeeded();
      }
      else {
        T* new_data = AllocData();
        if (data_) {
          for (uint64_t i = 0; i < size_; ++i) {
            new_data[i] = Lamp::move(data_[i]);
          }
        }
        delete[] data_;
        data_ = new_data;
      }

      size_ = _new_size;
    }

    void clear() {
      delete[] data_;

      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }
  };
} // namespace Lamp
#endif // LAMPVECTOR_H

#pragma once

#include <initializer_list>
#include <cstddef>
#include <iterator>
#include <utility>
#include <stdexcept>

#include "array_iterator.hpp"

static_assert(sizeof(char) == 1,
  "invalid implementation if size of type char not equal to one byte");

namespace stdlike {

template <typename T>
class Array {
 public:

  using iterator = ArrayIterator<T>;
  //using const_iterator = ArrayIterator<const T>;

  Array() {}
  explicit Array(size_t size);
  Array(size_t size, const T& init_value);
  Array(size_t size, T&& init_value);
  Array(std::initializer_list<T> list);

  Array(const Array&);
  Array& operator=(const Array&);

  Array(Array&&);
  Array& operator=(Array&&);

  T& operator[](size_t index);
  T& At(size_t index);

  size_t Size() const;
  size_t Capacity() const;

  void PushBack(const T&);
  void PushBack(T&&);

  template <typename... Args>
  void EmplaceBack(Args&&...);

  void Clear();

  void Resize(size_t);
  void Resize(size_t, const T&);
  void Resize(size_t, T&&);

  void Reserve(size_t);

  iterator Begin();
  iterator End();
/*
  const_iterator Begin() const;
  const_iterator End() const;
*/
  iterator begin() { return Begin(); }
  iterator end() { return End(); }
/*
  const_iterator begin() const { return Begin(); }
  const_iterator end() const { return End(); }
*/
  bool operator==(const Array&) const;
  bool operator!=(const Array&) const;

  ~Array();

 private:

  // Creation T only via this methods.
  // Even thoug T has default constructor,
  // we seperate memory allocation and call constructor
  // to avoid double destructor call
  // in cases when size_ < capacity_
  template <typename... Args>
  void Construct(T*, Args&&...);

  void Destroy(T*, size_t);

  T* Allocate(size_t);

  void Deallocate(T*);

  void Clear(T*&, size_t&, size_t&);

 private:
  T* arr_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
};

}


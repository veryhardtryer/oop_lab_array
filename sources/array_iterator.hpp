#pragma once

namespace stdlike {

template <typename T>
class Array;

template <typename T>
class ArrayIterator { //: public std::iterator<std::random_access_iterator_tag, T> {
 public:
  using iterator_category = std::random_access_iterator_tag; 
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

 friend class Array<T>;

 private:
  ArrayIterator(T* ptr) : ptr_(ptr) {}

 public:
  ArrayIterator& operator+=(int n) { ptr_ + n; return *this; }
  ArrayIterator& operator-=(int n) { ptr_ - n; return *this; }

  ArrayIterator operator+(int n)
  { return { ptr_ + n }; }

  ArrayIterator operator-(int n)
  { return { ptr_ - n }; }

  difference_type operator-(const ArrayIterator& other)
  { return static_cast<difference_type>(ptr_ - other.ptr_); }

  reference operator*() { return *ptr_; }
  reference operator[](int n) { return *(ptr_ + n); }
  pointer operator->() { return ptr_; }

  ArrayIterator& operator++() { ptr_++; return *this; }
  ArrayIterator operator++(int) { ArrayIterator tmp = *this; ptr_++; return tmp; }

  ArrayIterator& operator--() { ptr_--; return *this; }
  ArrayIterator operator--(int) { ArrayIterator tmp = *this; ptr_--; return tmp; }

  bool operator==(const ArrayIterator& other) const { return ptr_ == other.ptr_; }
  bool operator!=(const ArrayIterator& other) const { return ptr_ != other.ptr_; }

  bool operator<(const ArrayIterator& other) const { return ptr_ < other.ptr_; }
  bool operator>(const ArrayIterator& other) const { return ptr_ > other.ptr_; }

  bool operator<=(const ArrayIterator& other) const { return ptr_ <= other.ptr_; }
  bool operator>=(const ArrayIterator& other) const { return ptr_ >= other.ptr_; }

 private:
  value_type* ptr_;
};

}

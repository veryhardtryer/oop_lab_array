#include "dynamic_array.hpp"

namespace stdlike {

#define MIN_CAPACITY 10
#define RESIZE_COEFFICIENT 2

template <typename T>
Array<T>::Array(size_t size) : arr_(nullptr), size_(size), capacity_(size) {
  if(size == 0) { return; }
  if(capacity_ < MIN_CAPACITY) { capacity_ = MIN_CAPACITY; }
  arr_ = Allocate(capacity_);
  for(size_t i = 0; i < size_; ++i) {
    Construct(arr_ + i);
  }
}

template <typename T>
Array<T>::Array(size_t size, const T& init_value) :
    arr_(nullptr), size_(size), capacity_(size) {
  if(size == 0) { return; }
  if(capacity_ < MIN_CAPACITY) { capacity_ = MIN_CAPACITY; }

  arr_ = Allocate(capacity_);
  for(size_t i = 0; i < size; ++i) {
    Construct(arr_ + i, init_value);
  }
}

template <typename T>
Array<T>::Array(size_t size, T&& init_value) :
    arr_(nullptr), size_(size), capacity_(size) {
  if(size == 0) { return; }
  if(capacity_ < MIN_CAPACITY) { capacity_ = MIN_CAPACITY; }

  arr_ = Allocate(capacity_);
  for(size_t i = 0; i < size; ++i) {
    Construct(arr_ + i, std::move(init_value));
  }
}

template <typename T>
Array<T>::Array(std::initializer_list<T> list) {
  size_ = list.size();
  capacity_ = size_;
  if(capacity_ < MIN_CAPACITY) { capacity_ = MIN_CAPACITY; }
  arr_ = Allocate(capacity_);

  size_t i = 0;
  for(auto& element : list) {
    Construct(arr_ + i, std::move(element));
    ++i;
  }
}

template <typename T>
Array<T>::Array(const Array<T>& other) :
    size_(other.size_), capacity_(other.capacity_) {
  arr_ = Allocate(capacity_);
  for(size_t i = 0; i < size_; ++i) {
    arr_[i] = std::move(other.arr_[i]);
  }
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other) {
  if(this == &other) { return *this; }
  Clear();
  size_ = other.size_;
  capacity_ = other.capacity_;

  arr_ = Allocate(capacity_);
  for(size_t i = 0; i < size_; ++i) {
    arr_[i] = other.arr_[i];
  }
  return *this;
}

template <typename T>
Array<T>::Array(Array<T>&& other) :
    arr_(other.arr_), size_(other.size_), capacity_(other.capacity_) {
  other.arr_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <typename T>
Array<T>& Array<T>::operator=(Array<T>&& other) {
  Clear();

  arr_ = other.arr_;
  size_ = other.size_;
  capacity_ = other.capacity_;

  other.arr_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
  return *this;
}

template <typename T>
T& Array<T>::operator[](size_t index) {
  return arr_[index];
}

template <typename T>
T& Array<T>::At(size_t index) {
  if(index >= size_) {
    throw std::invalid_argument("Index can't be greater then size");
  }
  return arr_[index];
}

template <typename T>
size_t Array<T>::Size() const {
  return size_;
}

template <typename T>
size_t Array<T>::Capacity() const {
  return capacity_;
}

template <typename T>
void Array<T>::PushBack(const T& value) {
  EmplaceBack(value);
}

template <typename T>
void Array<T>::PushBack(T&& value) {
  EmplaceBack(std::move(value));
}

template <typename T>
template <typename... Args>
void Array<T>::EmplaceBack(Args&&... args) {
  if(size_ == capacity_) {
    Reserve(capacity_ * RESIZE_COEFFICIENT);
  }   
  Construct(arr_ + size_, std::forward<Args>(args)...);
  ++size_;
}

template <typename T>
void Array<T>::Clear() {
  Clear(arr_, size_, capacity_);
}

template <typename T>
void Array<T>::Clear(T*& ptr, size_t& size, size_t& capacity) {
  Destroy(ptr, size);
  Deallocate(ptr);
  ptr = nullptr;
  size = 0;
  capacity = 0;
}

template <typename T>
void Array<T>::Resize(size_t new_size) {
  if(new_size <= size_) { return; }
  if(new_size > capacity_) { //Bag
    Reserve(capacity_ * RESIZE_COEFFICIENT);
  }
  for(size_t i = size_; i < new_size; ++i) {
    Construct(arr_ + i);
  }
  size_ = new_size;
}

template <typename T>
void Array<T>::Resize(size_t new_size, const T& init_value) {
  if(new_size <= size_) { return; }
  if(new_size > capacity_) {
    Reserve(capacity_ * RESIZE_COEFFICIENT);
  }
  for(size_t i = size_; i < new_size; ++i) {
    Construct(arr_ + i, init_value);
  }
  size_ = new_size;
}

template <typename T>
void Array<T>::Resize(size_t new_size, T&& init_value) {
  if(new_size <= size_) { return; }
  if(new_size > capacity_) {
    Reserve(capacity_ * RESIZE_COEFFICIENT);
  }
  for(size_t i = size_; i < new_size; ++i) {
    Construct(arr_ + i, std::move(init_value));
  }
  size_ = new_size;
}

template <typename T>
void Array<T>::Reserve(size_t new_capacity) {
  if(new_capacity < MIN_CAPACITY) { new_capacity = MIN_CAPACITY; }
  if(new_capacity <= capacity_) { return; }

  size_t old_size = size_;
  size_t old_capacity = capacity_;
  T* old_arr = arr_;

  capacity_ = new_capacity;
  arr_ = Allocate(capacity_);
  for(size_t i = 0; i < size_; ++i) {
    Construct(arr_ + i, std::move(old_arr[i]));
  }

  Clear(old_arr, old_size, old_capacity);
}

#ifdef __GNUC__
template <typename T>
typename Array<T>::iterator Array<T>::Begin() {
  return iterator(arr_);
}

template <typename T>
typename Array<T>::iterator Array<T>::End() {
  return iterator(arr_ + size_);
}

template <typename T>
typename Array<T>::const_iterator Array<T>::Begin() const {
  return const_iterator(arr_);
}

template <typename T>
typename Array<T>::const_iterator Array<T>::End() const {
  return const_iterator(arr_ + size_);
}
#endif

template <typename T>
bool Array<T>::operator==(const Array<T>& other) const {
  if(size_ != other.size_) { return false; }

  for(size_t i = 0; i < size_; ++i) {
    if(arr_[i] != other.arr_[i]) { return false; }
  }
  return true;
}

template <typename T>
bool Array<T>::operator!=(const Array<T>& other) const {
  return !(*this == other);
}

template <typename T>
Array<T>::~Array() {
  Clear();
}

template <typename T>
template <typename... Args>
void Array<T>::Construct(T* ptr, Args&&... args) {
  new(ptr) T(std::forward<Args>(args)...); //Call constructor at address 'ptr'
}

template <typename T>
void Array<T>::Destroy(T* ptr, size_t count) {
  for(size_t i = 0; i < count; ++i) {
    ptr[i].~T(); //Excplicitly call destructor, don't free memory
  }
}
template <typename T> 
T* Array<T>::Allocate(size_t count) { 
  return reinterpret_cast<T*>(new char[count * sizeof(T)]); //Only allocate memory
}

template <typename T>
void Array<T>::Deallocate(T* ptr) {
  delete[] reinterpret_cast<char*>(ptr); //Free memory, call Destroy method before
}

}

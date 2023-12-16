#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>

#include "dynamic_array.hpp"
#include "dynamic_array.cpp"

using namespace stdlike;

struct MoveOnly {
  MoveOnly() {}
  //Non-copyable
  MoveOnly(const MoveOnly&) = delete;
  MoveOnly& operator=(const MoveOnly&) = delete;

  MoveOnly(MoveOnly&&) = default;
  MoveOnly& operator=(MoveOnly&&) = default;
};

struct NonDefaultConstruction {
  explicit NonDefaultConstruction(int n) : i(n) {}
  int i;
};

struct TwoStrings {
  TwoStrings(std::string arg1 = "", std::string arg2 = "", size_t arg3 = 0) 
    : s1(arg1), s2(arg2), i(arg3) {}

  std::string s1;
  std::string s2;
  size_t i;
};

void construction() {
  std::cout << "CONSTRUCTION\n";

  Array<int> a;
  assert(a.Size() == 0);
  assert(a.Capacity() == 0);

  Array<int> b(4);
  assert(b.Size() == 4);
  assert(b.Capacity() == MIN_CAPACITY);

  Array<int> c(11, 11);
  assert(c.Size() == 11);
  assert(c.Capacity() == 11);
  for(auto& element : c) {
    assert(element == 11);
  }

  Array<int> d{ 1, 2, 3 };
  assert(d.Size() == 3);
  assert(d.Capacity() == MIN_CAPACITY);
  assert(d[0] == 1);
  assert(d[1] == 2);
  assert(d[2] == 3);

  Array<int> e(d);
  assert(std::equal(d.begin(), d.end(), e.begin(), e.end()));
  Array<int> f(std::move(e));
  assert(std::equal(d.begin(), d.end(), f.begin(), f.end()));
  assert(e.Size() == 0);
  assert(e.Capacity() == 0);

  d = b; 
  assert(d.Size() == 4);
  assert(d.Capacity() == MIN_CAPACITY);

  a = std::move(b);
  assert(a.Size() == 4);
  assert(a.Capacity() == MIN_CAPACITY);

  assert(b.Size() == 0);
  assert(b.Capacity() == 0);
}

void at() {
  std::cout << "AT\n";

  Array<double> a { 2.0, 3.19 };
  try {
    a.At(3)++;
  } catch(std::invalid_argument& e) {
    return;
  }
  std::abort();
}

void resizeReserve() {
  std::cout << "RESIZE_AND_RESERVE\n";
  Array<int> a;
  a.Resize(5);
  assert(a.Size() == 5);
  assert(a.Capacity() == MIN_CAPACITY);
  
  a.Resize(4); 
  a.Reserve(9); 
  assert(a.Size() == 5);
  assert(a.Capacity() == MIN_CAPACITY);
 
  a.Clear(); 
  assert(a.Size() == 0);
  assert(a.Capacity() == 0);

  a.Reserve(11);
  assert(a.Size() == 0);
  assert(a.Capacity() == 11);
  for(size_t i = 0; i < 12; ++i) {
    a.PushBack(i);
  }
  assert(a.Size() == 12);
  assert(a.Capacity() == 11 * RESIZE_COEFFICIENT);
}

void pushBack() {
  std::cout << "PUSH_BACK\n";
  Array<size_t> a;
  for(size_t i = 0; i < 10000; ++i) {
    a.PushBack(i);
  }
  for(size_t i = 0; i < 10000; ++i) {
    assert(a[i] == i);
  }
}

void emplaceBack() {
  std::cout << "EMPLACE_BACK\n";
  Array<TwoStrings> a;
  std::vector<TwoStrings> v;
  std::string s1 = "I'm a movable string";
  std::string s2 = "I'm a copyable string";

  a.EmplaceBack(std::move(s1), s2, 0);
  assert(s1.size() == 0);
  assert(s2.size() != 0);

  {
    auto& obj = a[0];
    assert(obj.s1 == "I'm a movable string");
    assert(obj.s2 == "I'm a copyable string");
    assert(obj.i == 0);
  }

  a.Clear();

  for(size_t i = 0; i < 10000; ++i) {
    a.EmplaceBack("a", "aa", i);
  }

  for(size_t i = 0; i < 10000; ++i) {
    auto& obj = a[i];
    assert(obj.s1 == "a");
    assert(obj.s2 == "aa");
    assert(obj.i == i);
  } 
}

void iterators() {
  std::cout << "ITERATORS\n";

  Array<int> a { 3, 4, 1, -1, 0 };
  std::vector<int> v { 3, 4, 1, -1, 0 };

  std::sort(a.Begin(), a.End());
  std::sort(v.begin(), v.end());

  for(size_t i = 0; i < a.Size(); ++i) {
    assert(a[i] == v[i]);
  }

  auto a_max = std::max_element(a.Begin(), a.End());
  auto v_max = std::max_element(v.begin(), v.end());
  assert(*v_max == *a_max);

  auto a_min = std::min_element(a.Begin(), a.End());
  auto v_min = std::min_element(v.begin(), v.end());
  assert(*v_min == *a_min);
}

void compare() {
  std::cout << "COMPARE\n";
  Array<int> a { 0, 3, 2, -6 };
  Array<int> b(a);
  assert(b == a);
  std::sort(b.begin(), b.end());
  assert(b != a);
}

void moveOnly() {
  std::cout << "MOVE_ONLY\n";
  Array<MoveOnly> a;
  std::vector<MoveOnly> v;
  
  MoveOnly t;
  a.PushBack(std::move(t));
  a.EmplaceBack(std::move(t));

  std::vector<MoveOnly> v2(5);
  Array<MoveOnly> a2(5);
  v2 = std::move(v);
  a2 = std::move(a);

  a2.Resize(40, std::move(t));
}

void nonDefaultConstruction() {
  std::cout << "NON_DEFAULT_CONSTRUCTION\n";

  Array<NonDefaultConstruction> a(1, NonDefaultConstruction(5));
  std::vector<NonDefaultConstruction> v(1, NonDefaultConstruction(5));//for comprasion
/*
  Array<NonDefaultConstruction> a2(1); // Not valid Usage
  std::vector<NonDefaultConstruction> v2(1); 
*/
  {
    Array<NonDefaultConstruction> a2 { NonDefaultConstruction(1) }; // Ok
    std::vector<NonDefaultConstruction> v2 { NonDefaultConstruction(1) };
  }

  a.PushBack(NonDefaultConstruction(6));
  v.push_back(NonDefaultConstruction(6));

  a.EmplaceBack(8);
  v.emplace_back(8);
/*
  a.Resize(50); // Not valid usage
  v.resize(50);
*/
  NonDefaultConstruction o_a(10);
  NonDefaultConstruction o_v(10);
  a.Resize(15, o_a);
  a.Resize(18, std::move(o_a));

  v.resize(15, o_v);
  v.resize(18, std::move(o_v));

  a.Reserve(50);
  v.reserve(50); // Ok
  
  {
    Array<NonDefaultConstruction> a2 (a);
    std::vector<NonDefaultConstruction> v2 (v);
  }               

  a.Clear();

  for(int i = 0; i < 10000; ++i) {
    a.PushBack(NonDefaultConstruction(i));
  }

  for(int i = 0; i < 10000; ++i) {
    assert(a[i].i == i);
  }
}

void startTest(void(*func)()) {
  try {
    std::cout << "Test ";
    func();
    std::cout << "PASSED\n\n";
  } catch(const std::exception& e) {
    std::cout << "FAILD\n\n";
    std::cout << e.what();
  }
}

int main() {
  startTest(construction);
  startTest(at);
  startTest(resizeReserve);
  startTest(pushBack);
  startTest(emplaceBack);
  startTest(iterators);
  startTest(compare);
  startTest(moveOnly);
  startTest(nonDefaultConstruction);
  std::cout << "All tests passed\n";
  return 0;
}

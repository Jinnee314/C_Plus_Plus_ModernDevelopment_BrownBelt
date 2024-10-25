#include "MyUnitTestFreimwork.hpp"

#include <algorithm>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <memory>

using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();

    void Deallocate(T* object);

private:
    queue<unique_ptr<T>> free;
    set<unique_ptr<T>, less<>> allocated;  // Изменили на set
};

template <typename T>
T* ObjectPool<T>::Allocate() {
    if (free.empty()) {
        free.push(make_unique<T>());
    }
    auto ptr = move(free.front());
    free.pop();
    T* ret = ptr.get();
    allocated.insert(move(ptr));  // Изменили вставку
    return ret;
}

template <typename T>
T* ObjectPool<T>::TryAllocate() {
    if (free.empty()) {
        return nullptr;
    }
    return Allocate();
}

// Дописали функции сравнения
template<class T>
bool operator<(const unique_ptr<T>& lhs, const T* rhs) {
    return less<const T*>()(lhs.get(), rhs);
}
template<class T>
bool operator<(const T* lhs, const unique_ptr<T>& rhs) {
    return less<const T*>()(lhs, rhs.get());
}

template <typename T>
void ObjectPool<T>::Deallocate(T* object) {
    auto it = allocated.find(object);
    if (it == allocated.end()) {
        throw invalid_argument("");
    }
    // Изменили код перемещения
    free.push(move(allocated.extract(it).value()));
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//template <class T>
//class ObjectPool {
//public:
//    T* Allocate();
//    T* TryAllocate();
//
//    void Deallocate(T* object);
//
//private:
//    queue<unique_ptr<T>> free;
//    set<unique_ptr<T>, less<>> allocated;
//};
//
//template <typename T>
//T* ObjectPool<T>::Allocate() {
//    if (free.empty()) {
//        free.push(make_unique<T>());
//    }
//    auto [iter, isInsert] = allocated.insert(move(free.front()));
//    free.pop();
//    return (*iter).get();
//}
//
//template <typename T>
//T* ObjectPool<T>::TryAllocate() {
//    if (free.empty()) {
//        return nullptr;
//    }
//    return Allocate();
//}
//
//template<class T>
//bool operator<(const unique_ptr<T>& lhs, const T* rhs)
//{
//    return less<const T*>(lhs.get(), rhs);
//}
//
//template<class T>
//bool operator<(const T* lhs, const unique_ptr<T>& rhs)
//{
//    return less<const T*>(lhs, rhs.get());
//}
//
//template <typename T>
//void ObjectPool<T>::Deallocate(T* object) {
//    auto iter = allocated.find(object);
//    if (iter == allocated.end()) {
//        throw invalid_argument("");
//    }
//    auto resNoda = allocated.extract(iter);
//    free.push(move(resNoda.value()));
//}

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  MyUnitTest::TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}

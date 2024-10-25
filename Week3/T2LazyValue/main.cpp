#include "MyUnitTestFreimwork.hpp"

#include <functional>
#include <string>
#include <optional>
#include <mutex>

using namespace std;

template <typename T>
class LazyValue 
{
private:
    mutable optional<T> object;
    function<T()> init;
    mutable mutex lockInit; // для многопоточных программ

public:
    explicit LazyValue(std::function<T()> init) : init(move(init)) {}

    bool HasValue() const
    {
        return object.has_value();
    }
    const T& Get() const
    {
        if (lock_guard lg(lockInit); !object)
        {
            object = init();
        }
        return *object;
    }
};

void UseExample() {
    const string big_string = "Giant amounts of memory";

    LazyValue<string> lazy_string([&big_string] { return big_string; });

    ASSERT(!lazy_string.HasValue());
    ASSERT_EQUAL(lazy_string.Get(), big_string);
    ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
            });
    }
    ASSERT(!called);
}

int main() {
    MyUnitTest::TestRunner tr;
    RUN_TEST(tr, UseExample);
    RUN_TEST(tr, TestInitializerIsntCalled);
    return 0;
}
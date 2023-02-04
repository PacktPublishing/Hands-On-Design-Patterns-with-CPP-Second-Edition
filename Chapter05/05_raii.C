#include <stdlib.h>

#include "gtest/gtest.h"

struct object_counter {
    static int count;
    static int all_count;
    object_counter() { ++count; ++all_count; }
    ~object_counter() { --count; }
};
int object_counter::count = 0;
int object_counter::all_count = 0;

template <typename T> class raii {
    public:
    explicit raii(T* p) : p_(p) {}
    ~raii() { delete p_; }
    private:
    T* p_;
};

TEST(Memory, AcquireRelease) {
    object_counter::all_count = object_counter::count = 0;
    {
        raii p(new object_counter);
        EXPECT_EQ(1, object_counter::count);
        EXPECT_EQ(1, object_counter::all_count);
    }
    EXPECT_EQ(0, object_counter::count);
    EXPECT_EQ(1, object_counter::all_count);
}

struct my_exception {};

TEST(Memory, NoLeak) {
    object_counter::all_count = object_counter::count = 0;
    try {
        raii p(new object_counter);
        throw my_exception();
    } catch ( my_exception& e ) {
    }
    EXPECT_EQ(0, object_counter::count);
    EXPECT_EQ(1, object_counter::all_count);
}

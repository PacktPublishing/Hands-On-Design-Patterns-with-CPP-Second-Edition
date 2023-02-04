#include <vector>

#include "gtest/gtest.h"

TEST(Memory, Vector) {
    std::vector<int> v(10);
    EXPECT_EQ(10u, v.size());
    EXPECT_LE(10u, v.capacity());
}


#include <gtest/gtest.h>
#include "map.hh"

// Test fixture for map tests
class MapTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Basic test for add function
TEST_F(MapTest, AddPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(10, 20), 30);
}

TEST_F(MapTest, AddNegativeNumbers) {
    EXPECT_EQ(add(-5, -3), -8);
    EXPECT_EQ(add(-10, 10), 0);
}

TEST_F(MapTest, AddZero) {
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(5, 0), 5);
    EXPECT_EQ(add(0, 5), 5);
}

TEST_F(MapTest, AddLargeNumbers) {
    EXPECT_EQ(add(1000000, 2000000), 3000000);
}

#include <iostream>
#include <gtest/gtest.h>
#include <casimir/casimir.hpp>
#include <casimir/core/private-context.hpp>
#include <casimir/framework/tensor_shape.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;
using namespace core;
using namespace framework;

template<typename T>
bool operator==(const std::vector<T>& a, const std::vector<T>& b) {
	if (a.size() != b.size()) {
		return false;
	}
	for (cuint i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

TEST(TensorShape, ExtendedTest) {
	TensorShape shape({4,2,2});
	EXPECT_EQ(shape.rank(), 3);
	EXPECT_EQ(shape.size(), 16);
	EXPECT_TRUE(shape.coordinatesOf(0) == std::vector<cuint>({0, 0, 0}));
	EXPECT_TRUE(shape.coordinatesOf(1) == std::vector<cuint>({1, 0, 0}));
	EXPECT_TRUE(shape.coordinatesOf(4) == std::vector<cuint>({ 0, 1, 0 }));
	EXPECT_THROW(shape.coordinatesOf(16), Exception);
	EXPECT_THROW(shape.coordinatesOf(-1), Exception);
	EXPECT_TRUE(shape.offsetOf({ 0, 0, 0 }) == 0);
	EXPECT_TRUE(shape.offsetOf({ 1, 0, 0 }) == 1);
	EXPECT_THROW(shape.offsetOf({ 1, 0, 0, 0 }), Exception);
	EXPECT_THROW(shape.offsetOf({ 1, 0, 2 }), Exception);
	EXPECT_TRUE(shape.offsetOf(shape.coordinatesOf(11)) == 11);
	EXPECT_THROW(TensorShape({}), Exception);
	EXPECT_THROW(TensorShape({1,5,0}), Exception);
}

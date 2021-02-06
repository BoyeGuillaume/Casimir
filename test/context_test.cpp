#include <gtest/gtest.h>
#include <casimir/casimir.hpp>
#include <casimir/framework/datatype.hpp>
#include <casimir/core/private-context.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;
using namespace framework;

TEST(ContextTest, WithoutLogFile) {
	std::cout << "OK" << std::endl;
	EXPECT_NO_THROW(createContext(""));
	std::cout << "OK" << std::endl;
	CasimirContext ctx = createContext("");
	std::cout << "OK" << std::endl;
	EXPECT_NO_THROW(releaseContext(ctx));
	std::cout << "OK" << std::endl;
}

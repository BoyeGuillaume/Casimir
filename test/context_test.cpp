#include <gtest/gtest.h>
#include <casimir/casimir.hpp>
#include <casimir/framework/datatype.hpp>
#include <casimir/core/private-context.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;
using namespace framework;

TEST(ContextTest, WithoutLogFile) {
	EXPECT_NO_THROW(createContext(""));
	CasimirContext ctx = createContext("");
	EXPECT_NO_THROW(releaseContext(ctx));
}

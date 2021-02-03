#include <gtest/gtest.h>
#include <casimir/utilities/string.hpp>
#include <casimir/utilities/exception.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;

TEST(String, Equality) {
	EXPECT_TRUE(String("Hello world") == String("Hello world"));
	EXPECT_TRUE(String("I love you\n") == String("I love you\n"));
	EXPECT_TRUE(String('A', 10) == String("AAAAAAAAAA"));
	EXPECT_TRUE(String("I don't\0A1", 10) != String("I don't\0A1"));
	EXPECT_TRUE(String("I don't\0A1", 10) == String("I don't\0A1", 10));
	EXPECT_TRUE(String(String("Hello")) == "Hello");
}

TEST(String, Concatenation) {
	EXPECT_TRUE(String("Hello ") + String("World") == "Hello World");
	EXPECT_TRUE(String("Hello\0", 6) + String(" ?\u0004\u0002", 4) == String("Hello\0 ?\u0004\u0002", 10));
}

TEST(String, Convertion) {
	EXPECT_TRUE(String::toString((cint)1654) == "1654");
	EXPECT_TRUE(String::toString((cint)1 - 545) == "-544");
}

TEST(String, Length) {
	EXPECT_EQ(String("Hello world").length(), 11);
	EXPECT_EQ(String("\n\n\n\u0001").length(), 4);
	EXPECT_EQ(String("I have a dream.").length(), 15);
}

TEST(String, Append) {
	String A = "Hello world";
	A.append(5, 0x00);
	EXPECT_TRUE(String("Hello world\0\0\0\0\0", 16) == A);
}

TEST(String, Retrieve) {
	String ABC = "ABCDEFG";
	EXPECT_EQ(ABC[0], 'A');
	EXPECT_EQ(ABC[1], 'B');
	EXPECT_EQ(ABC[2], 'C');
	EXPECT_EQ(ABC[6], 'G');
	EXPECT_THROW(ABC[7], Exception);
	EXPECT_THROW(ABC[-1], Exception);

	ABC[0] = ' ';
	EXPECT_EQ(ABC[0], ' ');
	EXPECT_THROW(ABC[7] = '1', Exception);
}

TEST(String, StartAndEndWith) {
	EXPECT_TRUE(String("Hello world").startsWith("Hello"));
	EXPECT_TRUE(String("Hello world").endsWith("world"));
	EXPECT_TRUE(String("Hello world").endsWith(" world"));
	EXPECT_FALSE(String("Hello world").startsWith("Helloworld"));
	EXPECT_FALSE(String("Hello world").startsWith("hello"));
	EXPECT_FALSE(String("Hello world").startsWith("Hello world "));
	EXPECT_FALSE(String("Hello world").endsWith("Hello"));
	EXPECT_FALSE(String("Hello world").endsWith("Helloooooooooooo"));
}

TEST(String, Find) {
	EXPECT_EQ(String("I love having pancakes").findFirstOf("love"), 2);
	EXPECT_EQ(String("I AI IA IA").findFirstOf("IA"), 5);
	EXPECT_EQ(String("I AI IA IA").findFirstOf("IA", 5), 5);
	EXPECT_EQ(String("I AI IA IA").findFirstOf("IA", 6), 8);
	EXPECT_EQ(String("Nothing in here").findFirstOf("nothing"), String::notFound());
	EXPECT_EQ(String("world Hello world").findLastOf("world"), 12);
	EXPECT_EQ(String(" world Hello world").findLastOf("world", 11), 1);
	EXPECT_EQ(String(" world Hello world").findLastOf("W"), String::notFound());
	EXPECT_EQ(String("").findLastOf(""), 0);
	EXPECT_EQ(String("").findFirstOf(""), 0);
}

TEST(String, Replacement) {
	EXPECT_TRUE(String("I don't like this because thisthis th is is this this is not").replaceAll("this", "@")
		== String("I don't like @ because @@ th is is @ @ is not"));
}
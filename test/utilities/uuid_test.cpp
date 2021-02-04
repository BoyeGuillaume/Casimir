#include <gtest/gtest.h>
#include <casimir/casimir.hpp>
#include <casimir/utilities/exception.hpp>
#include <casimir/utilities/uuid.hpp>

using namespace Casimir;
using namespace literals;
using namespace utilities;

TEST(Uuid, Constructor) {
	EXPECT_TRUE(Uuid().isNIL());
	EXPECT_TRUE(Uuid(0, 0).isNIL());
	EXPECT_FALSE(Uuid(1, 1).isNIL());
}

TEST(Uuid, SerializeDeserialize) {
	EXPECT_TRUE(Uuid().formattedString() == "{00000000-0000-0000-0000-000000000000}");
	EXPECT_TRUE(Uuid(5156165181, 982928983).formattedString() == "{3dd65433-0100-0000-574e-963a00000000}");
	ASSERT_TRUE(Uuid::fromParsedString("{00000000-0000-0000-0000-000000000000}").isPresent());
	EXPECT_TRUE(Uuid::fromParsedString("{00000000-0000-0000-0000-000000000000}").get().isNIL());
	EXPECT_FALSE(Uuid::fromParsedString("Hello world how").isPresent());
	EXPECT_FALSE(Uuid::fromParsedString("{00000000-0000-0000-0000-0000000000}").isPresent());
	EXPECT_TRUE(Uuid::fromParsedString("{b4ae1b1e-141f-4f3b-b730-e7b4f86fb1e9}").get().formattedString() == "{b4ae1b1e-141f-4f3b-b730-e7b4f86fb1e9}");
}

TEST(Uuid, Operator) {
	EXPECT_TRUE(Uuid(0, 0) < Uuid(0, 1));
	EXPECT_TRUE(Uuid(0, 0) < Uuid(1, 0));
	EXPECT_TRUE(Uuid(0, 0) <= Uuid(0, 1));
	EXPECT_TRUE(Uuid(1, 0) > Uuid(0, 10));
	EXPECT_FALSE(Uuid() != Uuid(0, 0));
	EXPECT_FALSE(Uuid() > Uuid());
	EXPECT_FALSE(Uuid() < Uuid());
	EXPECT_TRUE(Uuid(0,1) < Uuid(0,2));
}

TEST(Uuid, CounterGenerator) {
	UuidCounterGenerator counter;
	EXPECT_FALSE(counter.nextUuid() == counter.nextUuid());
}

TEST(Uuid, RandomGenerator) {
	UuidRandomGenerator generator(478756056);
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{6737a733-70fe-9195-d960-7a4c1e6479da}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{fc2c3fda-7ea0-90dc-4f4c-735a3bb87853}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{1d03b95d-b7dc-2cc9-733d-f3cc9816909b}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{17ed9a64-f8e3-90f8-38f3-a64f8fec4f7e}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{90781b6e-90a3-acbf-9f9f-2bfa197e2e4e}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{8f8aa0b4-747e-7313-c5fd-25e2980a9a5e}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{45c31760-3226-6686-22a5-ad9d1ed1227f}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{ac02d713-8366-fe5f-fcc5-e8340c9ba5be}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{7ad03b5d-ddc6-ecf5-70d3-85bd8d7120b8}").get());
	EXPECT_TRUE(generator.nextUuid() == Uuid::fromParsedString("{3626526d-46f0-52e6-3672-c2f120eb07dc}").get());
}

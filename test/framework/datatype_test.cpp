#include <gtest/gtest.h>
#include <casimir/casimir.hpp>
#include <casimir/framework/datatype.hpp>
#include <casimir/core/private-context.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;
using namespace framework;

TEST(DataType, BasicTypeBuilder) {
	CasimirContext ctx = createContext("");
	try {
		DataType floatT = DataTypeBuilder()
			.setType(EDataType::Float)
			.create(ctx);
		EXPECT_EQ(floatT.dtype(), EDataType::Float);
		EXPECT_EQ(floatT.sizeOf(), sizeof(float));
		EXPECT_TRUE(floatT.at(Uuid()).isEmpty());
		EXPECT_FALSE(floatT.has(Uuid()));
		EXPECT_FALSE(floatT.isComplexType());

		DataTypeBuilder builder = DataTypeBuilder()
			.setType(EDataType::Int64);
		DataType int64T = builder.create(ctx);
		EXPECT_EQ(int64T.dtype(), EDataType::Int64);
		EXPECT_EQ(int64T.sizeOf(), sizeof(int64));

		EXPECT_THROW(builder.create(ctx), Exception);
		EXPECT_THROW(builder.asStructure(), Exception);
		EXPECT_THROW(builder.setType(EDataType::None), Exception);

		EXPECT_THROW(DataTypeBuilder().setType(EDataType::None).create(ctx), Exception);

		DataTypeBuilder sbuilder = DataTypeBuilder().setType(EDataType::Int32);
		sbuilder.asStructure();
		EXPECT_THROW(sbuilder.create(ctx), Exception);
	}
	catch (Exception e) {
		ctx->logger(PrivateLogging::Error) << e;
		throw e;
	}

	releaseContext(ctx);
}

TEST(DataType, ComplexTypeBuilder) {
	CasimirContext ctx = createContext("");
	try {
		DataType cType = DataTypeBuilder()
			.asStructure()
			.registerSubType(Uuid(1, 0), EDataType::Double)
			.registerSubType(Uuid(2, 0), EDataType::Float)
			.registerSubType(Uuid(2, 1), EDataType::Int32)
			.create(ctx);

		EXPECT_TRUE(cType.isComplexType());
		EXPECT_EQ(cType.dtype(), EDataType::Structure);
		EXPECT_EQ(cType.sizeOf(), 16);
		EXPECT_FALSE(cType.has(Uuid()));
		EXPECT_FALSE(cType.has(Uuid(1,1)));
		EXPECT_TRUE(cType.has(Uuid(2, 0)));
		EXPECT_EQ(cType.at(Uuid(1, 0)).get().dtype, EDataType::Double);
		EXPECT_EQ(cType.at(Uuid(2, 1)).get().dtype, EDataType::Int32);
		EXPECT_EQ(cType.at(Uuid(2, 1)).get().size, 4);
		EXPECT_EQ(cType.at(Uuid(2, 0)).get().offset, 8);
	}
	catch (Exception e) {
		ctx->logger(PrivateLogging::Error) << e;
		throw e;
	}

	releaseContext(ctx);
}

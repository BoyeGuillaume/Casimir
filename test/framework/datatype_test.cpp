#include <gtest/gtest.h>
#include <casimir/casimir.hpp>
#include <casimir/framework/datatype.hpp>
#include <casimir/core/private-context.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;
using namespace framework;

TEST(DataType, BasicTypeBuilder) {
	CasimirContext ctx = createContext(ContextConfiguration().withLogToShell(false));
	
	DataType floatT = DataTypeBuilder(ctx)
		.setType(EDataType::Float)
		.create();
	EXPECT_EQ(floatT.dtype(), EDataType::Float);
	EXPECT_EQ(floatT.sizeOf(), sizeof(float));
	EXPECT_TRUE(floatT.at(Uuid()).isEmpty());
	EXPECT_FALSE(floatT.has(Uuid()));
	EXPECT_FALSE(floatT.isComplexType());

	DataTypeBuilder builder = DataTypeBuilder(ctx)
		.setType(EDataType::Int64);
	DataType int64T = builder.create();
	EXPECT_EQ(int64T.dtype(), EDataType::Int64);
	EXPECT_EQ(int64T.sizeOf(), sizeof(int64));

	EXPECT_THROW(builder.create(), Exception);
	EXPECT_THROW(builder.asStructure(), Exception);
	EXPECT_THROW(builder.setType(EDataType::None), Exception);

	EXPECT_THROW(DataTypeBuilder(ctx).setType(EDataType::None).create(), Exception);

	DataTypeBuilder sbuilder = DataTypeBuilder(ctx).setType(EDataType::Int32);
	sbuilder.asStructure();
	EXPECT_THROW(sbuilder.create(), Exception);

	releaseContext(ctx);
}

TEST(DataType, ComplexTypeBuilder) {
	CasimirContext ctx = createContext(ContextConfiguration().withLogToShell(false));
	
	DataType cType = DataTypeBuilder(ctx)
		.asStructure()
		.registerSubType(Uuid(1, 0), EDataType::Double)
		.registerSubType(Uuid(2, 0), EDataType::Float)
		.registerSubType(Uuid(2, 1), EDataType::Int32)
		.create();

	EXPECT_TRUE(cType.isComplexType());
	EXPECT_EQ(cType.dtype(), EDataType::Structure);
	EXPECT_EQ(cType.sizeOf(), 16);
	EXPECT_FALSE(cType.has(Uuid()));
	EXPECT_FALSE(cType.has(Uuid(1,1)));
	EXPECT_TRUE(cType.has(Uuid(2, 0)));
	EXPECT_EQ(cType.at(Uuid(1, 0)).get().dtype(), EDataType::Double);
	EXPECT_EQ(cType.at(Uuid(2, 1)).get().dtype(), EDataType::Int32);
	EXPECT_EQ(cType.at(Uuid(2, 1)).get().sizeOf(), 4);
	EXPECT_EQ(cType.at(Uuid(2, 0)).get().offset(), 8);

	releaseContext(ctx);
}

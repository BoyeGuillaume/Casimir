#include <iostream>
#include <gtest/gtest.h>
#include <casimir/casimir.hpp>
#include <casimir/framework/datachunk.hpp>
#include <casimir/core/heap_allocator.hpp>
#include <casimir/core/heap_interface.hpp>
#include <casimir/core/private-context.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;
using namespace core;
using namespace framework;

TEST(DataChunk, WithoutDataOwning) {
	// Create the new RawData from the Heap allocator
	std::cout << "NOTE: the datachunk test will failed likely if the heap test doesn't pass" << std::endl;

	CasimirContext ctx = createContext(ContextConfiguration().withLogToShell(false));

	// Register the new interface
	HeapInterface* heapInterface = new HeapInterface(ctx, HeapConfiguration().withDebug(true).withDebugMemory(true));

	// Allocate new data block and set corresponding memory to 0
	RawData* data = heapInterface->allocator()->allocate(60);
	memset(data->data(), 0, data->size());

	// Instantiate the corresponding data chunk
	DataType dtype = DataTypeBuilder(ctx).setType(EDataType::Int32).create();
	
	// Create the firstFragment that occupy the data
	// [X X X X X - - - - - - - - - -] (32 bits)
	DataChunk* firstFragment = new DataChunk(ctx, data, dtype, 0, 6);
	
	// Basic assertion
	EXPECT_TRUE(firstFragment->dtype() == dtype);
	EXPECT_TRUE(firstFragment->isAllocated());
	EXPECT_FALSE(firstFragment->doesOwnData());

	// Assertion with pointer
	EXPECT_EQ(firstFragment->at(0), data->data());
	EXPECT_EQ(firstFragment->at(5), offsetOf(data->data(), 20));
	EXPECT_THROW(firstFragment->at(6), Exception);
	EXPECT_THROW(firstFragment->at(-1), Exception);
	EXPECT_THROW(firstFragment->free(), Exception);
	EXPECT_THROW(new DataChunk(ctx, data, dtype, 8, 5, 9), Exception);

	// Create fragment with slice
	// [- - X - - X - - X - - X - - X] (32 bits)
	DataChunk* slicedFragment = new DataChunk(ctx, data, dtype, 8, 5, 8);

	// In theory the pointer of the firstFragmet at id=5 must be equal to slicedFragment at id=1
	EXPECT_EQ(firstFragment->at(5), slicedFragment->at(1));
	EXPECT_EQ(slicedFragment->at(0), offsetOf(data->data(), 8));
	EXPECT_EQ(slicedFragment->at(4), offsetOf(data->data(), 56));

	delete slicedFragment;
	delete firstFragment;

	releaseContext(ctx);
}

TEST(DataChunk, WithDataOwning) {
	// Create the new RawData from the Heap allocator
	std::cout << "NOTE: the datachunk test will failed likely if the heap test doesn't pass" << std::endl;

	CasimirContext ctx = createContext(ContextConfiguration().withLogToShell(false));

	// Register the new interface
	HeapInterface* heapInterface = new HeapInterface(ctx, HeapConfiguration().withDebug(true).withDebugMemory(true));
	
	// All subtype uuid
	UuidCounterGenerator generator(1,1);
	const Uuid _a = generator.nextUuid();
	const Uuid _b = generator.nextUuid();
	const Uuid _c = generator.nextUuid();
	const Uuid _d = generator.nextUuid();

	// Instantiate the corresponding data chunk
	DataType dtype = DataTypeBuilder(ctx).asStructure()
		.registerSubType(_a, EDataType::Double)
		.registerSubType(_b, EDataType::Float)
		.registerSubType(_c, EDataType::Int32)
		.registerSubType(_d, EDataType::Int32)
		.create();

	// Therefore the dtype size if 8 + 4 + 4 + 4 = 20
	ASSERT_EQ(dtype.sizeOf(), 20);

	// Create the associate chunk
	DataChunk* chunk = new DataChunk(ctx, heapInterface->allocator(), dtype, 10);
	RawData* data = chunk->rawData();

	// Assert data is nullptr
	EXPECT_EQ(data, nullptr);
	EXPECT_TRUE(chunk->doesOwnData());
	EXPECT_FALSE(chunk->isAllocated());
	EXPECT_EQ(chunk->at(0), nullptr);

	// Allocate
	chunk->malloc();

	data = chunk->rawData();
	EXPECT_NE(data, nullptr);
	EXPECT_EQ(chunk->at(0), data->data());
	EXPECT_EQ(chunk->at(0, _d), offsetOf(data->data(), dtype.at(_d).get().offset()));
	EXPECT_EQ(chunk->at(0, _d), offsetOf(data->data(), 16));
	EXPECT_EQ(chunk->at(9), offsetOf(data->data(), 180));
	EXPECT_THROW(chunk->at(10), Exception);
	EXPECT_TRUE(chunk->isAllocated());
	
	// Free then realloc (just to see if no issues)
	chunk->free();
	chunk->malloc();
	chunk->free();
	chunk->malloc();
	delete chunk;

	releaseContext(ctx);
}

#include <gtest/gtest.h>
#include <casimir/core/heap_allocator.hpp>
#include <casimir/core/heap_interface.hpp>
#include <casimir/core/context.hpp>
#include <casimir/core/private-context.hpp>

using namespace Casimir;
using namespace literals;
using namespace framework;
using namespace core;
using namespace utilities;

TEST(Heap, HeapAllocation) {
	CasimirContext ctx = createContext(ContextConfiguration().withLogToShell(false));

	// Register the heap allocator
	HeapInterface* heapInterface = new HeapInterface(ctx, HeapConfiguration().withBlockAlignment(32));

	EXPECT_TRUE(heapInterface->uuid() == Heap);
	EXPECT_TRUE(heapInterface->allocator()->uuid() == Heap);

	// Test whether or not the allocator can correctly allocate the data
	EXPECT_EQ(heapInterface->allocator()->interface(), heapInterface);
	
	// Testing allocation
	RawData *db_root = heapInterface->allocator()->allocate(500);
	EXPECT_EQ(db_root->size(), 500);
	EXPECT_EQ(db_root->allocator(), heapInterface->allocator());
	
	// Try to write thing to the data buffer
	memset(db_root->data(), 0, db_root->size());

	// Free the allocator
	delete db_root;
	delete heapInterface;

	releaseContext(ctx);
}

TEST(Heap, HeapInternalCopy) {
	CasimirContext ctx = createContext(ContextConfiguration().withLogToShell(false));

	// Register the heap allocator
	HeapInterface* heapInterface = new HeapInterface(ctx, HeapConfiguration());

	// Then allocate raw Data
	RawData* data = heapInterface->allocator()->allocate(60);
	RawData* data2 = heapInterface->allocator()->allocate(80);

	// Data to be fill with
	memset(data->data(), 0, data->size());
	memset(data2->data(), 0, data2->size());

	// String that will be feed up in the data
	const char* dataString = "RvlP51wXUqdd8xbnC8eIB4zOOwwwCYOatqL64l9SEUtuk72Lda7G42YQaoC";
	const char* data2String = "jH53JTmkAwUoVSSjmMgwPa3nV36d1TT5V7fxlw1FJPAm5vz6ZKGkEYDaiikALinRLaBVUcYwByDRLOy";

	// Copy the data to the string
	memcpy(data->data(), dataString, 60);
	memcpy(data2->data(), data2String, 80);

	// Now use the copy function to copy from data to data2 with offset 40 20 and length 40
	AbstractAllocator::copy(data2, data, 40, 40, 20);

	// Compare memory with the expected result
	const char* expectedResult = "jH53JTmkAwUoVSSjmMgwPa3nV36d1TT5V7fxlw1FB4zOOwwwCYOatqL64l9SEUtuk72Lda7G42YQaoC";

	String test((char*) data2->data(), 80);
	ctx->logger(PrivateLogging::Note) << "Data destination value: " << test;

	ASSERT_EQ(memcmp(expectedResult, data2->data(), 80), 0);

	releaseContext(ctx);
}


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
	HeapInterface* heapInterface = new HeapInterface(ctx, HeapConfiguration());

	EXPECT_TRUE(heapInterface->uuid() == Heap);
	EXPECT_TRUE(heapInterface->allocator()->uuid() == Heap);

	// Test whether or not the allocator can correctly allocate the data
	EXPECT_EQ(heapInterface->allocator()->interface(), heapInterface);
	
	// Testing allocation
	DataBlock *db_root = heapInterface->allocator()->allocate(500);
	EXPECT_EQ(db_root->size(), 500);
	EXPECT_EQ(db_root->parentOrElseSelf(), db_root);
	EXPECT_EQ(db_root->allocator(), heapInterface->allocator());
	
	// Try to write thing to the data buffer
	memset(db_root->data(), 0, db_root->size());

	// Free the allocator
	delete db_root;

	releaseContext(ctx);
}



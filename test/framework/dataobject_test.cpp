#include <gtest/gtest.h>
#include <gtest/gtest.h>
#include <casimir/casimir.hpp>
#include <casimir/framework/datachunk.hpp>
#include <casimir/framework/dataobject.hpp>
#include <casimir/core/heap_allocator.hpp>
#include <casimir/core/heap_interface.hpp>
#include <casimir/core/private-context.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;
using namespace core;
using namespace framework;

TEST(DataObject, ConstructionAndAllocation) {
	// Create the new RawData from the Heap allocator
	std::cout << "NOTE: the datachunk test will failed likely if the heap test doesn't pass" << std::endl;

	// Create the new CasimirContext
	CasimirContext ctx = createContext(ContextConfiguration().withLogToShell(false));

	// Register the new interface
	new HeapInterface(ctx, HeapConfiguration().withDebug(true).withDebugMemory(true));

	// Create the type
	DataType dtype = DataTypeBuilder(ctx).setType(EDataType::Double).create();

	// Create the DataObject
	DataObject* object = DataObjectBuilder(ctx, dtype, 5)
		.registerInterface(Heap)
		.create();

	ASSERT_NE(object, nullptr);
	EXPECT_TRUE(object->has(Heap));
	EXPECT_FALSE(object->at(Heap)->isAllocated());
	EXPECT_TRUE(object->at(Heap)->dtype() == dtype);

	// Try to allocate
	object->malloc(Heap);
	EXPECT_TRUE(object->at(Heap)->isAllocated());
	object->free(Heap);
	EXPECT_FALSE(object->at(Heap)->isAllocated());
	EXPECT_THROW(object->at(Uuid(8, 8)), Exception);
	

	// Release the old context
	releaseContext(ctx);
}
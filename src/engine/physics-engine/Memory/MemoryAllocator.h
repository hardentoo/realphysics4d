#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H


// Libraries
#include <cstring>
#include "../config.h"

namespace real_physics
{

// Class MemoryAllocator
/**
 * This class is used to efficiently allocate memory on the heap.
 * It allows us to allocate small blocks of memory (smaller or equal to 1024 bytes)
 * efficiently. This implementation is inspired by the small block allocator
 * described here : http://www.codeproject.com/useritems/Small_Block_Allocator.asp
 */
class MemoryAllocator {

    private :

        // -------------------- Internal Classes -------------------- //

        // Structure MemoryUnit
        /**
         * Represent a memory unit that is used for a single memory allocation
         * request.
         */
        struct MemoryUnit {

            public :

                // -------------------- Attributes -------------------- //

                /// Pointer to the next memory unit inside a memory block
                MemoryUnit* nextUnit;

        };

        // Structure MemoryBlock
        /**
         * A memory block is a large piece of memory that is allocated once and that
         * will contain multiple memory unity.
         */
        struct MemoryBlock {

            public :

                /// Pointer to the first element of a linked-list of memory unity.
                MemoryUnit* memoryUnits;
        };

        // -------------------- Constants -------------------- //

        /// Number of heaps
        static const int NB_HEAPS = 128;

        /// Maximum memory unit size. An allocation request of a size smaller or equal to
        /// this size will be handled using the small block allocator. However, for an
        /// allocation request larger than the maximum block size, the standard malloc()
        /// will be used.
        static const size_t MAX_UNIT_SIZE = 1024;

        /// Size a memory chunk
        static const size_t BLOCK_SIZE = 16 * MAX_UNIT_SIZE;

        // -------------------- Attributes -------------------- //

        /// Size of the memory units that each heap is responsible to allocate
        static size_t mUnitSizes[NB_HEAPS];

        /// Lookup table that mape size to allocate to the index of the
        /// corresponding heap we will use for the allocation.
        static int mMapSizeToHeapIndex[MAX_UNIT_SIZE + 1];

        /// True if the mMapSizeToHeapIndex array has already been initialized
        static bool isMapSizeToHeadIndexInitialized;

        /// Pointers to the first free memory unit for each heap
        MemoryUnit* mFreeMemoryUnits[NB_HEAPS];

        /// All the allocated memory blocks
        MemoryBlock* mMemoryBlocks;

        /// Number of allocated memory blocks
        uint mNbAllocatedMemoryBlocks;

        /// Current number of used memory blocks
        uint mNbCurrentMemoryBlocks;

#ifndef NDEBUG
        /// This variable is incremented by one when the allocate() method has been
        /// called and decreased by one when the release() method has been called.
        /// This variable is used in debug mode to check that the allocate() and release()
        /// methods are called the same number of times
        int mNbTimesAllocateMethodCalled;
#endif

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        MemoryAllocator();

        /// Destructor
        ~MemoryAllocator();

        /// Allocate memory of a given size (in bytes) and return a pointer to the
        /// allocated memory.
        void* allocate(size_t size);

        /// Release previously allocated memory.
        void release(void* pointer, size_t size);

};


}

#endif // MEMORYALLOCATOR_H

#ifndef CASIMIR_HEAPINTERFACE_HPP_
#define CASIMIR_HEAPINTERFACE_HPP_

#include "../casimir.hpp"
#include "../framework/interface.hpp"
#include "../utilities/uuid.hpp"
#include "heap_allocator.hpp"


namespace Casimir::core {
    
    class HeapAllocator;
    class HeapInterface;
    
    /**
     * @brief Heap configuration storage-class that enable to defines a specific configuration for the HeapAllocator
     */
    class HeapConfiguration {
    private:
        bool m_doesKeepTrackOfDataChunk = true;
        bool m_debugMemory = true;
        bool m_debug = true;
        cuint m_blockAlignment = 0;
    
    public:
        /**
         * @brief Does the data chunk are tracked internally
         * @return Whether or not the DataBlocks allocate by the HeapAllocator are tracked in a internal list
         */
        inline bool doesKeepTrackOfDataChunk() const {
            return m_doesKeepTrackOfDataChunk;
        }
    
        /**
         * @brief Does the allocator log each allocation and free process
         * @return Whether or not the allocator log each memory operations
         */
        inline bool debugMemory() const {
            return m_debugMemory && m_debug;
        }
    
        /**
         * @brief Does the allocator / interface log anything that happen. This is a necessary condition to debugMemory()
         * @return Whether or not the allocator log note and information to the log
         */
        inline bool debug() const {
            return m_debug;
        }
    
        /**
         * @brief Each framework::RawData are aligned to a specific power of 2
         * @return The value of the above parameters
         */
        inline cuint blockAlignment() const {
            return m_blockAlignment;
        }
        
        /**
         * @brief Keep track of each framework::RawData allocator by the HeapAllocator to an internal list
         * @param keepTrackOfDataChunk The value of the parameters
         * @return A self-reference
         */
        inline HeapConfiguration& withKeepTrackOfDataChunk(bool keepTrackOfDataChunk) {
            m_doesKeepTrackOfDataChunk = keepTrackOfDataChunk;
            return *this;
        }
        
        /**
         * @brief Does the allocator / interface log anything that happen. This is a necessary condition to debugMemory()
         * @param value The value of the parameters
         * @return A self-reference
         */
        inline HeapConfiguration& withDebug(bool value) {
            m_debug = value;
            return *this;
        }
    
        /**
         * @brief Does the allocator log each memory operations
         * @param doesDebugMemory The value of the parameters
         * @return A self-reference
         */
        inline HeapConfiguration& withDebugMemory(bool doesDebugMemory) {
            m_debugMemory = doesDebugMemory;
            return *this;
        }
        
        /**
         * @brief Each framework::RawData are aligned to a specific power of 2
         * @param blockAlignment The value of the parameters. If 0 then the blocks are allocate with default alignment
         * (using the allocate function)
         * @return A self-reference
         */
        inline HeapConfiguration& withBlockAlignment(cuint blockAlignment) {
            m_blockAlignment = blockAlignment;
            return *this;
        }
    };
    
    /**
     * @brief The HeapInterface defines the most basic interface that is the CPU
     */
    class HeapInterface : public framework::AbstractInterface {
        CASIMIR_DISABLE_COPY_MOVE(HeapInterface)
        friend class HeapAllocator;
    
    private:
        HeapAllocator* m_allocator;
        HeapConfiguration m_config;
    
    public:
        /**
         * @brief Default constructor with HeapConfiguration.
         * @param ctx The interface is an object that live in a specific CasimirContext object
         * @param config The configuration used to instantiate the current interface
         */
        CASIMIR_EXPORT HeapInterface(CasimirContext ctx, HeapConfiguration config);
        
        /**
         * @brief Destruct the current heap interface (as long as the associate allocator)
         */
        CASIMIR_EXPORT ~HeapInterface() override;
        
        /**
         * @brief Retrieve the allocator associate to the HeapInterface
         * @return The allocator linked to the current interface (HeapAllocator)
         */
        CASIMIR_EXPORT framework::AbstractAllocator* allocator() const override;
        
        /**
         * @brief Retrieve the name of the current interface
         * @return A String that defines the current allocator
         */
        CASIMIR_EXPORT utilities::String name() const override;
        
        /**
         * @brief Return a String that describe the current state of the HeapInterface
         * @return A utilities::String that describe the current object
         */
        CASIMIR_EXPORT utilities::String toString() const override;
    };
    
    /**
     * @brief Constant value that characterise the Heap (interface). This value is the uuid of the HeapInterface and
     * of the HeapAllocator and can be used to retrieve the HeapInterface from a list of AbstractInterface
     */
    static constexpr utilities::Uuid Heap = utilities::Uuid(15945639145337867237U,12112052614447901516U);
    
}

#endif

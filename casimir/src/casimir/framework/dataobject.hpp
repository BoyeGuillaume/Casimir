#ifndef CASIMIR_DATAOBJECT_HPP_
#define CASIMIR_DATAOBJECT_HPP_

#include <unordered_map>

#include "../casimir.hpp"
#include "datachunk.hpp"
#include "allocator.hpp"
#include "interface.hpp"
#include "contextual_object.hpp"
#include "../utilities/uuid.hpp"
#include "../utilities/string_serializable.hpp"
#include "../utilities/string.hpp"
#include "../utilities/exception.hpp"

namespace Casimir::framework {

    /**
     * @brief Data object is a class that hold a list of DataChunk of same size and type of different interface
     */
    class DataObject : public ContextualObject, public utilities::StringSerializable {
        CASIMIR_DISABLE_COPY_MOVE(DataObject)

    private:
        std::unordered_map<utilities::Uuid,DataChunk*> m_chunkById;
        
    public:
        /**
         * @brief Default constructor of DataObject
         * @param ctx The context that must be given to any ContextualObject
         * @param chunks A list of chunks hold by the DataObject
         * @throw utilities::Exception if the chunks doesn't share the same context or doesn't have the same size
         * of doesn't have the same type
         */
        CASIMIR_EXPORT explicit DataObject(CasimirContext ctx, std::vector<DataChunk*> chunks);
        
        /**
         * @brief Default destructor
         */
        CASIMIR_EXPORT virtual ~DataObject();
        
        /**
         * @brief Copy an chunk from a source interface to a destination interface
         * @param destination The `uuid` of the destination interface (where data are copied to)
         * @param source The `uuid` of the source interface (where data are copied from)
         */
        CASIMIR_EXPORT void copy(const utilities::Uuid& destination, const utilities::Uuid& source);
        
        /**
         * @brief Determine if the current DataObject hold an given interface
         * @param interface The `uuid` of the interface
         * @return Whether or not there's a chunk that is linked to the given interface in the current object
         */
        CASIMIR_EXPORT bool has(const utilities::Uuid& interface) const;
        
        /**
         * @brief Return the DataChunk object at the given interface
         * @param interface The `uuid` of the interface we want to retrieve the chunk
         * @throw utilities::Exception if no interface has been found under the given uuid in the current object
         * @return The DataChunk linked to the given interface
         */
        CASIMIR_EXPORT DataChunk* at(const utilities::Uuid& interface) const;
    
        /**
         * @brief Allocate the DataChunk associate to the given interface
         * @param interface The `uuid` of the interface we will allocate
         * @throw utilities::Exception if the interface as not been found or if the allocation process failed
         */
        inline void malloc(const utilities::Uuid& interface) {
            at(interface)->malloc();
        }
        
        /**
         * @brief Release the DataChunk associate to the given interface
         * @param interface The `uuid` of the interface we will free
         * @throw utilities::Exception if the interface as not been found or if the fre process failed
         */
         inline void free(const utilities::Uuid& interface) {
             at(interface)->free();
         }
        
    
        /**
          * @brief Return the DataChunk object at the given interface
          * @param interface The `uuid` of the interface we want to retrieve the chunk
          * @throw utilities::Exception if no interface has been found under the given uuid in the current object
          * @return The DataChunk linked to the given interface
          */
        inline DataChunk* operator[](const utilities::Uuid& interface) const {
            return at(interface);
        }
        
        /**
         * @brief Return the pointer at the given interface with the given index
         * @param interface The `uuid` of the interface we want to retrieve the pointer
         * @param index The index of the pointer
         * @throw utilities::Exception if no interface has been found under the given uuid in the current object or
         * if the given index is out of range
         * @return The pointer to the data in the given interface at the given index
         */
        inline void* at(const utilities::Uuid& interface, cuint index) const {
            return at(interface)->at(index);
        }
        
        /**
         * @brief Return the pointer at the given interface with the given index of a given parameters
         * @param interface The `uuid` of the interface we want to retrieve the pointer
         * @param index The index of the pointer
         * @param parameters The `uuid` of the parameters (in the structure)
         * @throw utilities::Exception if no interface has been found under the given uuid in the current object or
         * if the given index is out of range or if the parameters is found
         * @return The pointer to the data in the given interface at the given index with the given field
         */
        inline void* at(const utilities::Uuid& interface, cuint index, const utilities::Uuid& parameters) const {
            return at(interface)->at(index, parameters);
        }
        
        /**
         * @brief Gave information about the current DataObject
         * @return A String that describe the current state of the object
         */
        CASIMIR_EXPORT utilities::String toString() const override;
    };
    
    /**
     * @brief Simple class that allocate the required spaces for any registered interface and return a data object
     */
    class DataObjectBuilder : public ContextualObject {
    private:
        std::shared_ptr<std::vector<DataChunk*>> m_chunks;
        DataType m_dataType;
        cuint m_length;

    public:
        /**
         * @brief Default constructor of the builder
         * @param ctx The context in which the current builder live
         * @param dataType The dataType of the output DataObject
         * @param length The length of the list we wanna create
         */
        inline explicit DataObjectBuilder(CasimirContext ctx, const DataType& dataType, cuint length)
        : ContextualObject(ctx),
        m_chunks(std::make_shared<std::vector<DataChunk*>>()),
        m_dataType(dataType),
        m_length(length)
        {}
        
        /**
         * @brief Default destructor of the builder. Free the allocated chunk if no object has been created with
         */
        CASIMIR_EXPORT virtual ~DataObjectBuilder();
        
        /**
         * @brief Register a DataChunk linked to the given interface to the output object
         * @param interface `uuid` of the interface (registered in the context) that will be used
         * @return A self-reference
         */
        CASIMIR_EXPORT DataObjectBuilder& registerInterface(const utilities::Uuid& interface);
        
        /**
         * @brief Create the DataObject with the given configuration. Notice that this operation will remove all the
         * previous allocated interface
         * @return The pointer to the new instance of the DataObject created from the configuration
         */
        CASIMIR_EXPORT DataObject* create();
        
    };

}

#endif

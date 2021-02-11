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
namespace Casimir::framework {

    /**
     * @brief Data object is a class that hold a list of DataChunk of same size and type of different interface
     */
    class DataObject : public ContextualObject, public utilities::StringSerializable {
        CASIMIR_DISABLE_COPY_MOVE(DataObject)

    private:
        std::unordered_map<utilities::Uuid,DataChunk> m_chunkById;
        std::vector<DataChunk> m_chunkObject;
        
    public:
        /**
         * @brief Default constructor of DataObject
         * @param ctx The context that must be given to any ContextualObject
         * @param chunks A list of chunks hold by the DataObject
         * @throw utilities::Exception if the chunks doesn't share the same context or doesn't have the same size
         * of doesn't have the same type
         */
        CASIMIR_EXPORT explicit DataObject(CasimirContext ctx, std::vector<DataChunk> chunks);
        
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
        CASIMIR_EXPORT DataChunk at(const utilities::Uuid& interface) const;
    
        /**
          * @brief Return the DataChunk object at the given interface
          * @param interface The `uuid` of the interface we want to retrieve the chunk
          * @throw utilities::Exception if no interface has been found under the given uuid in the current object
          * @return The DataChunk linked to the given interface
          */
        inline DataChunk operator[](const utilities::Uuid& interface) const {
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
            return at(interface).at(index);
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
            return at(interface).at(index, parameters);
        }
        
        /**
         * @brief Gave information about the current DataObject
         * @return A String that describe the current state of the object
         */
        CASIMIR_EXPORT utilities::String toString() const override;
        
    };

}

#endif

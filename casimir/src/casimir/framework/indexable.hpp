#ifndef CASIMIR_INDEXABLE_HPP_
#define CASIMIR_INDEXABLE_HPP_

#include "../casimir.hpp"
#include "../utilities/uuid.hpp"

namespace Casimir::framework {

    /**
     * @brief Defines an object that contains an Uuid and can be indexed as a key of a unordered_map or a hash map
     * (Only if movable and copyable)
     *
     * Notice that an IndexableObject is an immutable object and cannot be change after the construction
     */
    class IndexableObject {
    private:
        const utilities::Uuid m_uuid;

    public:
        /**
         * @brief Default constructor take the uuid as input
         * @param uuid The utilities::Uuid that hold the index of the current object
         */
        CASIMIR_EXPORT explicit IndexableObject(utilities::Uuid uuid);
        
        /**
         * @brief Equality operator that compare the index of two IndexableObject
         * @param other the other instance of IndexableObject to be compared to
         * @return Whether or not the two instance has the same index
         */
        inline bool operator==(const IndexableObject& other) const {
            return m_uuid == other.m_uuid;
        }
        
        /**
         * @brief Retrieve the index of the current object
         * @return The utilities::Uuid of the current object that represent it's index
         */
        inline utilities::Uuid uuid() const {
            return m_uuid;
        }
        
        /**
         * @brief Return an index object from the current IndexableObject instance.
         *
         * A Index object has the same index as the current object but is movable and copyable. Using this trick we can
         * build map onto non-copyable entities
         *
         * @return An new IndexableObject that has the same utilities::Uuid than the current instance
         */
        inline IndexableObject index() const {
            return IndexableObject(m_uuid);
        }
    };

}

namespace std {
  
    /**
     * @brief Hash structure implementation for any IndexableObject that are copyable
     */
    template<>
    struct hash<Casimir::framework::IndexableObject> {
        CASIMIR_EXPORT size_t operator()(const Casimir::framework::IndexableObject& indexableObject) const;
    };
    
    /**
    * @brief Hash structure implementation for any pairs of IndexableObject pointer
    */
    template<>struct hash<std::pair<Casimir::framework::IndexableObject, Casimir::framework::IndexableObject>> {
        CASIMIR_EXPORT size_t operator()(const std::pair<Casimir::framework::IndexableObject,
                Casimir::framework::IndexableObject>& pair) const;
    };
    
}

#endif

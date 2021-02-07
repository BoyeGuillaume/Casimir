#include "indexable.hpp"

#include <utility>

namespace Casimir::framework {
    
    CASIMIR_EXPORT IndexableObject::IndexableObject(utilities::Uuid uuid)
            : m_uuid(std::move(uuid)) {}
    
}


CASIMIR_EXPORT std::size_t
std::hash<Casimir::framework::IndexableObject>::operator()(const Casimir::framework::IndexableObject& indexableObject) const {
    return std::hash<Casimir::utilities::Uuid>()(indexableObject.uuid());
}

CASIMIR_EXPORT size_t std::hash<std::pair<Casimir::framework::IndexableObject, Casimir::framework::IndexableObject>>::operator()(
        const std::pair<Casimir::framework::IndexableObject, Casimir::framework::IndexableObject>& pair) const {
    return std::hash<Casimir::framework::IndexableObject>()(pair.first) ^
           std::hash<Casimir::framework::IndexableObject>()(pair.second);
}
#include "tensor_shape.hpp"
#include "../utilities/exception.hpp"

#include <cmath>
#include <algorithm> // std::transform
#include <iterator>  // std::back_inserter

namespace Casimir::framework {
    
    using namespace utilities;
    using namespace literals;
    
    CASIMIR_EXPORT TensorShape::TensorShape(const std::vector<cuint>& shapes)
    : m_shapes(shapes) {
        // Retrieve the size by performing the product other the shapes
        cuint size = 1;
        for(cuint dim : shapes) {
            size *= dim;
        }
        m_size = size;

#ifdef CASIMIR_SAFE_CHECK
        if(m_shapes.empty() || size == 0) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot create a empty tensor. Not supported operation");
        }
#endif
    }
    
    CASIMIR_EXPORT cuint TensorShape::offsetOf(const std::vector<cuint>& coordinates) const {
#ifdef CASIMIR_SAFE_CHECK
        if(coordinates.size() != m_shapes.size()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "The given coordinates doesn't have the required shape");
        }
#endif
    
        cuint currentOffset = 0;
        cuint currentSlice  = 1;
        for (cuint i = 0; i < m_shapes.size(); ++i) {
#ifdef CASIMIR_SAFE_CHECK
            if(coordinates[i] >= m_shapes[i]) {
                CASIMIR_THROW_EXCEPTION("IndexOufOfRange", "The given coordinates index is out of range");
            }
#endif
            // Increment the offset using the current slice
            currentOffset += coordinates[i] * currentSlice;
            currentSlice  *= m_shapes[i];
        }
        
        return currentOffset;
    }
    
    CASIMIR_EXPORT std::vector<cuint> TensorShape::coordinatesOf(cuint offset) {
#ifdef CASIMIR_SAFE_CHECK
        if(offset >= m_size) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "The given offset is out of the range and doesn't correspond to anything");
        }
#endif
        
        std::vector<cuint> coordinates(m_shapes.size(), 0U);
        
        // Reverse iterator
        cuint currentSlice = m_size;
        cuint currentOffset = offset;
        for (cint i = m_shapes.size() - 1; i >= 0; --i) {
            currentSlice /= m_shapes[i];
            coordinates[i] = (cuint) std::floor(((double) currentOffset) / ((double) currentSlice));
            currentOffset -= coordinates[i] * currentSlice;
        }
        
        return coordinates;
    }
    
    CASIMIR_EXPORT utilities::String TensorShape::toString() const {
        std::vector<String> values;
        std::transform(m_shapes.begin(), m_shapes.end(), std::back_inserter(values), [](auto& kv) {
            return String::toString((cint) kv);
        });
        return "TensorShape({" + String(", ").join(values) + "})";
    }
    
    
}


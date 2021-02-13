#ifndef CASIMIR_TENSOR_SHAPE_HPP_
#define CASIMIR_TENSOR_SHAPE_HPP_

#include <vector>

#include "../casimir.hpp"
#include "../utilities/string.hpp"
#include "../utilities/string_serializable.hpp"

namespace Casimir::framework {
    
    /**
     * @brief TensorShape defines the space that can take a tensor object. It is also used to retrieve the offset from
     * a coordinates
     */
    class TensorShape : public utilities::StringSerializable {
    private:
        std::vector<cuint> m_shapes;
        cuint m_size;
        
    public:
        /**
         * @brief Default constructor that directly take the shapes as a list
         * @param shapes A list of cuint that defines the dimension of the tensor. For instance
         * a 2 by 4 matrices will be created from shape {2,4}
         * @throw utilities::Exception if the shapes is empty (doesn't support empty shape)
         */
        CASIMIR_EXPORT explicit TensorShape(const std::vector<cuint>& shapes);
        
        /**
         * @brief Retrieve the offset (in elements number) from the starting coordinates (first offset is 0) corresponding
         * to a given coordinates
         * @param coordinates A list of the coordinates that we wanted to retrieve from
         * @throw utilities::Exception if the given coordinates list isn't valid (doesn't have the same size as the rank()
         * or one of the coordinates is out of range)
         * @return The offset that correspond to the given `coordinates`
         */
        CASIMIR_EXPORT cuint offsetOf(const std::vector<cuint>& coordinates) const;
        
        /**
         * @brief Retrieve the position that correspond to a given offset
         * @param offset The offset we wanted to retrieve the position
         * @throw utilities::Exception if the given offset isn't contains in the range of the current shape
         * @return The coordinates of the given offset
         */
        CASIMIR_EXPORT std::vector<cuint> coordinatesOf(cuint offset);
        
        /**
         * @brief Retrieve a String that describe the internal state of the current object
         * @return A String that describe the current TensorShape object
         */
        CASIMIR_EXPORT utilities::String toString() const override;
    
        /**
         * @brief The size of a given Shape is defines as the number of elements that could fit in the Tensor
         *
         * We can see the `size` of a given TensorShape as the size of the flat map that correspond to the Tensor
         *
         * @return The size of the given TensorShape
         */
        inline cuint size() const {
            return m_size;
        }
    
        /**
         * @brief Retrieve the rank of the given tensor (number of coordinates that describe each points)
         * @return The rank of the current TensorShape
         */
        inline cuint rank() const {
            return m_shapes.size();
        }
        
    };
}

#endif

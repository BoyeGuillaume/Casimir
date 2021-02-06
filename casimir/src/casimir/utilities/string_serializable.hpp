#ifndef CASIMIR_STRING_SERIALIZABLE_HPP_
#define CASIMIR_STRING_SERIALIZABLE_HPP_

#include "../casimir.hpp"

namespace Casimir {

    namespace utilities {

        class String;

        /**
         * @brief Abstract class that defines an object that can be converted to a String
         */
        class StringSerializable {
        public:
            /**
             * @brief abstract method that convert the current object to a String
             * @return A String that describe the object in the current state
             */
            virtual String toString() const = 0;
        };

    };

};

#endif

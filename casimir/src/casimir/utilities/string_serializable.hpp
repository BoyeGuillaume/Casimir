#ifndef CASIMIR_STRING_SERIALIZABLE_HPP_
#define CASIMIR_STRING_SERIALIZABLE_HPP_

#include "../casimir.hpp"

namespace Casimir {

    namespace utilities {

        class String;

        class StringSerializable {
        public:
            virtual String toString() const = 0;
        };

    };

};

#endif

#ifndef CASIMIR_UUID_HPP_
#define CASIMIR_UUID_HPP_

#include "../casimir.hpp"
#include "string.hpp"

namespace Casimir {

    namespace utilities {

        class Uuid {
        private:
            ubyte m_rawData[16];

        public:
            CASIMIR_EXPORT Uuid();

            CASIMIR_EXPORT Uuid(ubyte rawData[16]);

            CASIMIR_EXPORT Uuid(uint64 mostSignificant, uint64 lessSignificant);

            CASIMIR_EXPORT static Uuid fromRawString(String rawString);

            CASIMIR_EXPORT static Uuid fromParsedString(String parsedString);

            CASIMIR_EXPORT String formattedString() const;

            inline String data() const {
                return String((const char*) &m_rawData[0], 16);
            }

            inline const ubyte* rawData() const {
                return &m_rawData[0];
            }
        };

    }

    namespace literals {
        CASIMIR_EXPORT bool operator==(const utilities::Uuid& a, const utilities::Uuid& b);
        CASIMIR_EXPORT bool operator>(const utilities::Uuid& a, const utilities::Uuid& b);
        CASIMIR_EXPORT bool operator<(const utilities::Uuid& a, const utilities::Uuid& b);
        CASIMIR_EXPORT bool operator<=(const utilities::Uuid& a, const utilities::Uuid& b);
        CASIMIR_EXPORT bool operator>=(const utilities::Uuid& a, const utilities::Uuid& b);

        inline bool operator!=(const utilities::Uuid& a, const utilities::Uuid& b) {
            return !operator==(a, b);
        }
    }
};


#endif //CASIMIR_UUID_HPP_

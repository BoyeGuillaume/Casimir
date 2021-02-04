#ifndef CASIMIR_UUID_HPP_
#define CASIMIR_UUID_HPP_

#include <hash_map>

#include "../casimir.hpp"
#include "string.hpp"
#include "string_serializable.hpp"
#include "optional.hpp"

namespace Casimir {

    namespace utilities {

        /**
         * @brief The \link Uuid class defines an Universal Unique Identifier that can be compare serialize from and to a string...
         */
        class Uuid {
        private:
            ubyte m_rawData[16];

        public:
            /**
             * @brief Default Uuid constructor (create a NIL Uuid)
             */
            CASIMIR_EXPORT Uuid();

            /**
             * @brief Construct an new instance of Uuid based on come rawData
             * @warning The rawData must have at least 16 bytes (only 16 bytes will be read)
             * @param rawData The raw data used to construct the Uuid (MUST BE 16 BYTES)
             */
            CASIMIR_EXPORT explicit Uuid(const ubyte* rawData) noexcept;

            /**
             * @brief Create a new Uuid based on two x64 integer
             * @param mostSignificant the first integer used to construct the Uuid (mostSignificant one)
             * @param lessSignificant the second integer used to construct the Uuid (lessSignificant one)
             */
            CASIMIR_EXPORT Uuid(const uint64& mostSignificant, const uint64& lessSignificant);

            /**
             * @brief Construct a new \link utilities::Uuid from a \link utilities::String containing the raw data
             * @param rawString the raw data string used to construct the Uuid
             * @return An Optional \link utilities::Uuid result of the raw data. If the rawString's length isn't equal to 16
             * it will considered the string as invalid and will return an empty Optional
             */
            CASIMIR_EXPORT static Optional<Uuid> fromRawString(const String& rawString);

            /**
             * @brief Construct a new Uuid from a parsed \link utilities::String
             * @param parsedString the parsed String used to create the Uuid
             * @return An Optional Uuid that return the new \link utilities::Uuid if the `parsedString` is valid otherwise will return
             * an empty \link utilities::Optional
             */
            CASIMIR_EXPORT static Optional<Uuid> fromParsedString(const String& parsedString);

            /**
             * @brief Return a formatted version of the Uuid of format {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
             * @return the resulting format as a \link utilities::String
             */
            CASIMIR_EXPORT String formattedString() const;

            /**
             * @brief Check equality between two Uuid
             * @param other the second Uuid to be compare to
             * @return Whether or not both Uuid are equals
             */
            CASIMIR_EXPORT bool operator==(const Uuid& other) const;

            /**
             * @brief Check inequality between two Uuid
             * @param other the second Uuid to be compare to
             * @return Whether or not both Uuid are unequals
             */
            inline bool operator!=(const Uuid& other) const {
                return !(this->operator==(other));
            }

            /**
             * @brief Return the data hold by this Uuid
             * @return A String that contains the data hold by the current Uuid (not granted to be C-String style as
             * can have null bytes)
             */
            inline String data() const {
                return String((char*) &m_rawData[0], 16);
            }

            /**
             * @brief Return the data hold by this Uuid
             * @return A pointer to the data of this current instance. The data is 16 bytes wide.
             */
            inline const ubyte* rawData() const {
                return &m_rawData[0];
            }

            /**
             * @brief Return a pointer to the most significant part of the Uuid
             * @return A pointer to the most significant part of the Uuid
             */
            inline const uint64* mostSignificant() const {
                return (uint64*) rawData();
            }

            /**
             * @brief Return a pointer to the less significant part of the Uuid
             * @return A pointer to the less significant part of the Uuid
             */
            inline const uint64* lessSignificant() const {
                return mostSignificant() + 1;
            }
        };

    }

    namespace literals {
        CASIMIR_EXPORT bool operator>(const utilities::Uuid& a, const utilities::Uuid& b);
        CASIMIR_EXPORT bool operator<(const utilities::Uuid& a, const utilities::Uuid& b);
        CASIMIR_EXPORT bool operator>=(const utilities::Uuid& a, const utilities::Uuid& b);
        CASIMIR_EXPORT bool operator<=(const utilities::Uuid& a, const utilities::Uuid& b);
    }
}

namespace std {
    /**
     * @brief Defines the hash of the Uuid as defines above (enable to use it as a key in a hash_map for instance)
     */
    template<>
    struct hash<Casimir::utilities::Uuid> {
        std::size_t operator()(const Casimir::utilities::Uuid& uuid) const {
            return std::hash<Casimir::uint64>()(*uuid.mostSignificant()) ^
                    std::hash<Casimir::uint64>()(*uuid.lessSignificant());
        }
    };
}

#endif

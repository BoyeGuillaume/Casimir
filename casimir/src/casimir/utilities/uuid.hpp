#ifndef CASIMIR_UUID_HPP_
#define CASIMIR_UUID_HPP_

#include <functional>
#include <random>

#include "../casimir.hpp"
#include "string.hpp"
#include "string_serializable.hpp"
#include "optional.hpp"

namespace Casimir {

    namespace utilities {

        /**
         * @brief The Uuid class defines an Universal Unique Identifier that can be compare serialize from and to a string...
         */
        class Uuid : public StringSerializable {
        private:
            union {
                ubyte m_rawData[16];
                struct {
                    uint64 mostSignificant;
                    uint64 lessSignificant;
                } bytes;
            };

        public:
            /**
             * @brief Default Uuid constructor (create a NIL Uuid)
             */
            inline constexpr Uuid()
            : bytes{0U,0U} {}

            /**
             * @brief Construct an new instance of Uuid based on come rawData
             * @warning The rawData must have at least 16 bytes (only 16 bytes will be read)
             * @param rawData The raw data used to construct the Uuid (MUST BE 16 BYTES)
             */
            inline constexpr explicit Uuid(const ubyte* rawData) noexcept
            : bytes{*((uint64*) rawData), *(((uint64*) rawData) + 1)} {}

            /**
             * @brief Create a new Uuid based on two x64 integer
             * @param mostSignificant the first integer used to construct the Uuid (mostSignificant one)
             * @param lessSignificant the second integer used to construct the Uuid (lessSignificant one)
             */
            inline constexpr Uuid(const uint64& mostSignificant, const uint64& lessSignificant)
            : bytes{mostSignificant, lessSignificant} {}

            /**
             * @brief Construct a new utilities::Uuid from a utilities::String containing the raw data
             * @param rawString the raw data string used to construct the Uuid
             * @return An Optional utilities::Uuid result of the raw data. If the rawString's length isn't equal to 16
             * it will considered the string as invalid and will return an empty Optional
             */
            CASIMIR_EXPORT static Optional<Uuid> fromRawString(const String& rawString);

            /**
             * @brief Construct a new Uuid from a parsed utilities::String
             * @param parsedString the parsed String used to create the Uuid
             * @return An Optional Uuid that return the new utilities::Uuid if the `parsedString` is valid otherwise will return
             * an empty utilities::Optional
             */
            CASIMIR_EXPORT static Optional<Uuid> fromParsedString(const String& parsedString);

            /**
             * @brief Return a formatted version of the Uuid of format {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
             * @return the resulting format as a utilities::String
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
                return &bytes.mostSignificant;
            }

            /**
             * @brief Return a pointer to the less significant part of the Uuid
             * @return A pointer to the less significant part of the Uuid
             */
            inline const uint64* lessSignificant() const {
                return &bytes.lessSignificant;
            }

            /**
             * @brief Return whether or not the current instance is the NIL instance
             * @return whether or not the current uuid is NIL
             */
            inline bool isNIL() const {
                return *mostSignificant() == 0 && *lessSignificant() == 0;
            }

            /**
             * @brief Convert the current Uuid to a string
             * @return a string that describe the current state of the Uuid object
             */
            inline String toString() const override {
                return literals::operator+("Uuid(", literals::operator+(formattedString(), ")"));
            }
        };

        /**
         * @brief A random generator that generate a new Uuid every single time
         */
        class UuidRandomGenerator {
        private:
            std::mt19937_64 m_generator;
            std::uniform_int_distribution<uint64> m_distribution;

        public:
            /**
             * @brief seeded-constructor of UuidRandomGenerator. Two different instance of UuidRandomGenerator
             * with the same `seed` will always shared the same behavior
             * @param seed an uint64 that defines the `seed` of the pseudo random number generator system
             */
            inline explicit UuidRandomGenerator(uint64 seed)
            : m_generator(seed), m_distribution(0, std::numeric_limits<uint64>::max()) {}

            /**
             * @brief default constructor of UuidRandomGenerator (with random seed)
             */
            inline UuidRandomGenerator() : UuidRandomGenerator(std::random_device()()) {}

            /**
             * @brief Generate the next Uuid using the random generation device
             * @return The utilities::Uuid that has been create by the current instance
             * @note The given Uuid is randomly generate and change every single times this function is called (really likely)
             */
            inline Uuid nextUuid() {
                return Uuid(m_distribution(m_generator), m_distribution(m_generator));
            }
        };

        /**
         * @brief A counter Uuid generation that keep a count of each already generated Uuid and therefore assure
         * the uniqueness of each resulting Uuid
         */
        class UuidCounterGenerator {
        private:
            uint64 m_lessSignificant, m_mostSignificant;

        public:
            /**
             * @brief Constructor of UuidCounterGenerator with value
             * @param lessSignificant the lessSignificant value where we start to generate the Uuid
             * @param mostSignificant the mostSignificant value where we start to generate the Uuid
             */
            inline UuidCounterGenerator(uint64 lessSignificant, uint64 mostSignificant)
            : m_lessSignificant(lessSignificant), m_mostSignificant(mostSignificant) {}

            /**
             * @brief Default constructor of UuidCounterGenerate that start the generation at 0
             */
            inline UuidCounterGenerator() : UuidCounterGenerator(0,0) {}

            /**
             * @brief Generate the next Uuid and increment the internal value that represent the current Uuid
             * @return A unique new Uuid() generate from the current count in memory
             */
            inline Uuid nextUuid() {
                // Slightly overkill since the condition is satisfied every 20 trillion call so....
                if(++m_lessSignificant == 0) ++m_mostSignificant;
                return Uuid(m_mostSignificant, m_lessSignificant);
            }
        };

    }

    namespace literals {
        /**
         * @brief Operator greater between two Uuid
         * @param a the first Uuid to be compared
         * @param b the second Uuid to be compared
         * @return whether or not `a` is strictly greater than `b`
         */
        CASIMIR_EXPORT bool operator>(const utilities::Uuid& a, const utilities::Uuid& b);

        /**
         * @brief Operator less between two Uuid
         * @param a the first Uuid to be compared
         * @param b the second Uuid to be compared
         * @return whether or not `a` is strictly less than `b`
         */
        CASIMIR_EXPORT bool operator<(const utilities::Uuid& a, const utilities::Uuid& b);

        /**
         * @brief Operator greater or equal between two Uuid
         * @param a the first Uuid to be compared
         * @param b the second Uuid to be compared
         * @return whether or not `a` is greater than or equal to `b`
         */
        CASIMIR_EXPORT bool operator>=(const utilities::Uuid& a, const utilities::Uuid& b);

        /**
         * @brief Operator less or equal between two Uuid
         * @param a the first Uuid to be compared
         * @param b the second Uuid to be compared
         * @return whether or not `a` is less than or equal to `b`
         */
        CASIMIR_EXPORT bool operator<=(const utilities::Uuid& a, const utilities::Uuid& b);

    }
}

namespace std {
    /**
     * @brief Defines the hash of the Uuid as defines above (enable to use it as a key in a hash_map for instance)
     */
    template<>
    struct hash<Casimir::utilities::Uuid> {
        CASIMIR_EXPORT std::size_t operator()(const Casimir::utilities::Uuid& uuid) const;
    };
}

#endif

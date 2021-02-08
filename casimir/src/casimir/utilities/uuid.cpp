#include "uuid.hpp"

#include <cstring>

namespace Casimir {

    using namespace literals;
   
    CASIMIR_EXPORT utilities::Optional<utilities::Uuid> utilities::Uuid::fromRawString(const utilities::String &rawString) {
        if(rawString.length() != 16) {
            return utilities::Optional<utilities::Uuid>::empty();
        }

        return utilities::Optional<utilities::Uuid>::of(Uuid((ubyte*) rawString.c_str()));
    }

    CASIMIR_EXPORT utilities::Optional<utilities::Uuid> utilities::Uuid::fromParsedString(const utilities::String &parsedString) {
        String rawHexString = parsedString
                .replaceAll("{", "")
                .replaceAll("}", "")
                .replaceAll("-", "")
                .decodeFromHex();

        // Return the Uuid using the raw string constructor
        return fromRawString(rawHexString);
    }

    CASIMIR_EXPORT utilities::String utilities::Uuid::formattedString() const {
        String hexString = String((char*) m_rawData, 16).encodeToHex();
        hexString.insert(8, "-");
        hexString.insert(13, "-");
        hexString.insert(18, "-");
        hexString.insert(23, "-");
        return "{" + hexString + "}";
    }

    CASIMIR_EXPORT bool utilities::Uuid::operator==(const utilities::Uuid &other) const {
        return memcmp(m_rawData, other.m_rawData, 16) == 0;
    }

    CASIMIR_EXPORT bool literals::operator>(const utilities::Uuid &a, const utilities::Uuid &b) {
        return memcmp(a.rawData(), b.rawData(), 16) > 0;
    }

    CASIMIR_EXPORT bool literals::operator<(const utilities::Uuid &a, const utilities::Uuid &b) {
        return memcmp(a.rawData(), b.rawData(), 16) < 0;
    }

    CASIMIR_EXPORT bool literals::operator>=(const utilities::Uuid &a, const utilities::Uuid &b) {
        return memcmp(a.rawData(), b.rawData(), 16) >= 0;
    }

    CASIMIR_EXPORT bool literals::operator<=(const utilities::Uuid &a, const utilities::Uuid &b) {
        return memcmp(a.rawData(), b.rawData(), 16) <= 0;
    }

};

CASIMIR_EXPORT std::size_t std::hash<Casimir::utilities::Uuid>::operator()(const Casimir::utilities::Uuid &uuid) const {
    return std::hash<Casimir::uint64>()(*uuid.mostSignificant()) ^
           std::hash<Casimir::uint64>()(*uuid.lessSignificant());
}


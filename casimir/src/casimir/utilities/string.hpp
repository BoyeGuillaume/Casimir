#ifndef CASIMIR_STRING_HPP_
#define CASIMIR_STRING_HPP_

#include <string>
#include <utility>
#include <vector>
#include <limits>

#include "../casimir.hpp"
#include "string_serializable.hpp"

namespace Casimir {

    namespace utilities {

        class String {
        private:
            std::string m_str;

        public:
            inline String() = default;

            inline String(const char value, cuint size)
                : m_str(size, (size_t) value)
            {}

            inline String(const char* str)
                : m_str(str)
            {}

            inline String(const char* byte, cuint size)
                : m_str(byte, (size_t) size)
            {}

            inline String(std::string str)
                : m_str(std::move(str))
            {}

            inline String(const StringSerializable& serializable)
                : String(serializable.toString())
            {}

            inline void append(const std::string& str) {
                m_str.append(str);
            }

            inline void append(const String& str) {
                m_str.append(str.m_str);
            }

            inline void append(const char* str) {
                m_str.append(str);
            }

            inline void append(cuint count, const char value) {
                m_str.append(count, value);
            }

            inline cuint length() const {
                return (cuint) m_str.length();
            }

            inline std::string str() const {
                return m_str;
            }

            inline const char* c_str() const {
                return m_str.c_str();
            }

            inline bool operator==(const String& str) {
                return str.m_str == m_str;
            }

            inline bool operator!=(const String& str) {
                return str.m_str != m_str;
            }
            
            inline void operator+=(const String& str) {
                append(str);
            }

            inline static constexpr cuint notFound() {
                return std::numeric_limits<cuint>::max();
            }

            CASIMIR_EXPORT cuint findFirstOf(const String& research, const cuint& afterPos) const;

            CASIMIR_EXPORT cuint findLastOf(const String& research, const cuint& beforePos) const;

            inline cuint findFirstOf(const String& research) const {
                return findFirstOf(research,0);
            }

            inline cuint findLastOf(const String& research) {
                return findLastOf(research, length() - 1);
            }

            CASIMIR_EXPORT String substr(const cuint& start, const cuint& length) const;

            CASIMIR_EXPORT char& at(const cuint& pos);

            inline char& operator[](const cuint& pos) {
                return at(pos);
            }

            CASIMIR_EXPORT char at(const cuint& pos) const;

            inline char operator[](const cuint& pos) const {
                return at(pos);
            }

            CASIMIR_EXPORT std::vector<String> split(const String& separator, bool discardEmptyStrings = false) const;

            CASIMIR_EXPORT bool startsWith(const String& str) const;

            CASIMIR_EXPORT bool endsWith(const String& str) const;

            CASIMIR_EXPORT String join(std::vector<String> list) const;
            
            CASIMIR_EXPORT String replaceAll(const String& str, const String& replacement) const;

            static String toString(cint value) {
                return String(std::to_string(value));
            }

            static String toString(double value) {
                return String(std::to_string(value));
            }

            static String toString(float value) {
                return String(std::to_string(value));
            }
        };

    }

    namespace literals {
        CASIMIR_EXPORT utilities::String operator+(const utilities::String& a, const utilities::String& b);
    }

}

#endif

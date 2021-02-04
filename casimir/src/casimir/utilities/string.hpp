#ifndef CASIMIR_STRING_HPP_
#define CASIMIR_STRING_HPP_

#include <string>
#include <utility>
#include <vector>
#include <limits>
#include <algorithm>

#include "../casimir.hpp"
#include "string_serializable.hpp"

namespace Casimir {

    namespace utilities {

        /**
         * @brief String class that is used in the whole Casimir project
         */
        class String {
        private:
            std::string m_str;

        public:
            /**
             * @brief Default String constructor
             */
            inline String() = default;

            /**
             * @brief Construct a string based on a single value repeated multiple times
             * @param value the value used to construct the string
             * @param count the number of times the value is repeated to form the String
             */
            inline String(const char value, cuint count)
                : m_str(count, (size_t) value)
            {}

            /**
             * @brief Construct a string based on a C-String
             * @param str the C-String used to construct the string
             */
            inline String(const char* str)
                : m_str(str)
            {}

            /**
             * @brief Construct a string based on data and a size
             * @param byte the data used to construct the string
             * @param size the size of the data
             */
            inline String(const char* byte, cuint size)
                : m_str(byte, (size_t) size)
            {}

            /**
             * @brief Construct a string based on a \link std::string
             * @param str the \link std::string use to construct the \link Casimir::utilities::String
             */
            inline String(std::string str)
                : m_str(std::move(str))
            {}

            /**
             * @brief Construct a string based on a \link utilities::StringSerializable object
             * @param serializable the serializable object
             */
            inline String(const StringSerializable& serializable)
                : String(serializable.toString())
            {}

            /**
             * @brief Convert a value to a String
             * @param value the `value` to be converted to a \link utilities::String
             * @return the resulting string
             */
            static String toString(cint value) {
                return String(std::to_string(value));
            }

            /**
             * @brief Convert a value to a String
             * @param value the `value` to be converted to a \link utilities::String
             * @return the resulting string
             */
            static String toString(double value) {
                return String(std::to_string(value));
            }

            /**
             * @brief Convert a value to a String
             * @param value the `value` to be converted to a \link utilities::String
             * @return the resulting string
             */
            static String toString(float value) {
                return String(std::to_string(value));
            }

            /**
             * @brief Convert a `string` to a String. (Notice this trivial function is used in case of template function)
             * @param value the `value` to be converted to a \link utilities::String
             * @return the resulting string
             */
             static String toString(String value) {
                 return value;
             }

            /**
             * @brief Append a \link std::string at the end of the current string
             * @param str the string append
             */
            inline void append(const std::string& str) {
                m_str.append(str);
            }

            /**
             * @brief Append a \link Casimir::utilities::String at the end of the current string
             * @param str the string append
             */
            inline void append(const String& str) {
                m_str.append(str.m_str);
            }

            /**
             * @brief Append a C-String at the end of the current string
             * @param str the string append
             */
            inline void append(const char* str) {
                m_str.append(str);
            }

            /**
             * @brief Append a single value multiple times to the string
             * @param count the number of times the value is added
             * @param value the value to be appended to the string
             */
            inline void append(cuint count, const char value) {
                m_str.append(count, value);
            }

            /**
             * @brief Get the length of the current String
             * @return  the length of the current String
             */
            inline cuint length() const {
                return (cuint) m_str.length();
            }

            /**
             * @brief Convert the \link utilities::String object to a \link std::string object
             * @return An equivalent std::string object
             */
            inline std::string str() const {
                return m_str;
            }

            /**
             * @brief Convert the current instance to a C-Style string
             * @return A C-Style string that is safe to use as long as the current instance is alive
             */
            inline const char* c_str() const {
                return m_str.c_str();
            }

            /**
             * @brief Equality operator between two \link utilities::String
             * @param str The second String we are comparing to
             * @return Whether or not the two string are equivalent
             */
            inline bool operator==(const String& str) {
                return str.m_str == m_str;
            }

            /**
             * @brief Non-equality operator between two \link utilities::String
             * @param str The second String we are comparing to
             * @return Whether or not the two string are different
             */
            inline bool operator!=(const String& str) {
                return str.m_str != m_str;
            }

            /**
             * @brief Append a String to the end of the current instance
             * @param str The string appended at the end of the current instance
             */
            inline void operator+=(const String& str) {
                append(str);
            }

            /**
             * @brief Static constant that correspond to the value returned by the following method if no
             * occurrence is found
             * @return a constant that defines the no found behavior
             */
            inline static constexpr cuint notFound() {
                return std::numeric_limits<cuint>::max();
            }

            /**
             * @brief Find the first occurrences of the research \link utilities::String from the position `afterPos`
             * @param research The sub-string to find in the current instance
             * @param afterPos The position where we start the research
             * @return The position of the characters before the first occurrence of `research` after the afterPos. If
             * no occurrence is found return the <code>String::notFound()</code> constant
             */
            CASIMIR_EXPORT cuint findFirstOf(const String& research, const cuint& afterPos) const;

            /**
             * @brief Find the last occurrences of the research \link utilities::String before the position `beforePos`
             * @param research The sub-string to find in the current instance
             * @param beforePos The position where we stop the research
             * @return The position of the characters before the last occurrence of `research` before the beforePos. If
             * no occurrence is found return the <code>String::notFound()</code> constant
             */
            CASIMIR_EXPORT cuint findLastOf(const String& research, const cuint& beforePos) const;

            /**
             * @brief Find first occurrence of research
             * @param research The sub-string to find in the current instance
             * @return The position of the characters before the first occurrence of `research`. If
             * no occurrence is found return the <code>String::notFound()</code> constant
             */
            inline cuint findFirstOf(const String& research) const {
                return findFirstOf(research,0);
            }

            /**
             * @brief Find last occurrence of research
             * @param research The sub-string to find in the current instance
             * @return The position of the characters before the last occurrence of `research`. If
             * no occurrence is found return the <code>String::notFound()</code> constant
             */
            inline cuint findLastOf(const String& research) {
                return findLastOf(research, length() - 1);
            }

            /**
             * @brief Extract the sub-string that start at position `start` and has length `length`
             * @param start the starting position of the sub-string we are considering
             * @param length the length of the sub-string we are considering
             * @throw utilities::Exception if the sub-string isn't contained in the current string
             * @return the resulting sub-string
             */
            CASIMIR_EXPORT String substr(const cuint& start, const cuint& length) const;

            /**
             * @brief Get character at position `pos`
             * @param pos the character position
             * @return A reference to the given character
             */
            CASIMIR_EXPORT char& at(const cuint& pos);

            /**
             * @brief Get character at position `pos`
             * @param pos the character position
             * @return A reference to the given character
             */
            inline char& operator[](const cuint& pos) {
                return at(pos);
            }

            /**
             * @brief Get character at position `pos`
             * @param pos the character position
             * @return A copy of the given character
             */
            CASIMIR_EXPORT char at(const cuint& pos) const;

            /**
             * @brief Get character at position `pos`
             * @param pos the character position
             * @return A copy of the given character
             */
            inline char operator[](const cuint& pos) const {
                return at(pos);
            }

            /**
             * @brief Split the string over a given separator token
             * @param separator The separator token (the string is split on each occurrence of this argument)
             * @param discardEmptyStrings Defines the behavior of the empty string. If an empty string is detected and
             * this argument is set to true then the empty string will be ignored
             * @return An \link std::vector<utilities::String> of the split string
             */
            CASIMIR_EXPORT std::vector<String> split(const String& separator, bool discardEmptyStrings = false) const;

            /**
             * @brief Check if the string start with the given `str`
             * @param str the `str` to use for the check
             * @return Whether or not the string start with the given sequence
             */
            CASIMIR_EXPORT bool startsWith(const String& str) const;

            /**
             * @brief Check if the string end with the given `str`
             * @param str the `str` to use for the check
             * @return Whether or not the string end with the given sequence
             */
            CASIMIR_EXPORT bool endsWith(const String& str) const;

            /**
             * @brief Join a vector of String using the current instance as a separator
             * @param list the list of string to be join using the current instance as separator
             * @param discardEmptyString Whether or not empty string are discarded
             * @return The resulting join string
             */
            CASIMIR_EXPORT String join(std::vector<String> list, bool discardEmptyString = true) const;

            /**
             * @brief Join a list of argument convertible to String to a unique string using current instance as
             * separator
             * @tparam Args the Arguments type
             * @param args variadic argument of all the parameters to be used
             * @return The resulting join string
             */
            template<typename... Args>
            String join(Args... args) const {
                std::vector<String> result;
                auto initList = {args...};
                using T = typename decltype(initList)::value_type;
                std::vector<T> expanded{initList};
                result.resize(expanded.size());
                std::transform(expanded.begin(), expanded.end(), result.begin(), [](T value){return String::toString(value);});
                return join(result, true);
            }

            /**
             * @brief Replace all occurrence of `str` with `replacement`
             * @param str The String to be replaced
             * @param replacement The replacement String
             * @return The resulting String
             */
            CASIMIR_EXPORT String replaceAll(const String& str, const String& replacement) const;
        };

    }

    namespace literals {
        /**
         * @brief Concatenate two String `a` and `b`
         * @param a the first String to be concatenate
         * @param b the second String to be concatenate
         * @return the resulting String consisting of `a` followed by `b`
         */
        CASIMIR_EXPORT utilities::String operator+(const utilities::String& a, const utilities::String& b);

#ifdef CASIMIR_LITERAL_OPERATOR
        /**
         * @brief Instantiate String using a literal separator
         * @param a the C-Style string to be used to construct the resulting
         * @param len the length of the C-Style string (therefore can support null-byte)
         * @return the resulting \link utilities::String
         */
        inline utilities::String operator "" _str (const char* a, std::size_t len) {
            return utilities::String(a, (cuint) len);
        }
#endif
    }

}

#endif

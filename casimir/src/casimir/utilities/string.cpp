#include "string.hpp"
#include "exception.hpp"

#include <cstring>

namespace Casimir {

    CASIMIR_EXPORT cuint utilities::String::findFirstOf(const utilities::String& research, const cuint& afterPos) const {
        // If the length of the research is null then simply return `afterPos`
        if(research.length() == 0) return afterPos;
        
        // Loop over the m_str from `afterPos` until the current string sequence match the required characters
        for (cuint i = afterPos; i < length(); ++i) {
            // Check if the substring from `i` to `i + research.length()` match the research string
            if(memcmp(c_str() + i, research.c_str(), research.length()) == 0) {
                return i; // We have found the first occurrence
            }
        }

        // If the program reach this line this means that no occurrence of `research` has been detected in the string
        return notFound();
    }

    CASIMIR_EXPORT cuint utilities::String::findLastOf(const utilities::String& research, const cuint& beforePos) const {
        // In the case were the String is empty
        if (length() == 0)
            return (research.length() == 0) ? (0) : (notFound());

        // Assert that the beforePos is a valid position (otherwise throw exception)
#ifdef CASIMIR_SAFE_CHECK
        if(beforePos >= length()) CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot find the last of the given"
                                                                             "string because the beforePosition is out of range");
#endif
        // If the length of the research is null then simply return `beforePos`
        if(research.length() == 0) return beforePos;
        
        // Loop over the m_str from `beforePos` until it reach 0
        for (cuint i = beforePos + 1; i >= research.length(); --i) {
            // Check if the substring from `i - research.length() + 1` to `i`
            if(memcmp(c_str() + i - research.length(), research.c_str(), research.length()) == 0) {
                return i - research.length();
            }
        }
        
        // If the program reach this line this means that no occurrence of `research` has been detected in the string
        return notFound();
    }
    
    CASIMIR_EXPORT utilities::String utilities::String::substr(const cuint &start, const cuint &length) const {
#ifdef CASIMIR_SAFE_CHECK
      if(start + length > this->length()) {
          CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot perform the given operation"
                                                     "as the specified region isn't fully contained by the string");
      }
#endif
        return String(m_str.substr(start, length));
    }
    
    CASIMIR_EXPORT char &utilities::String::at(const cuint &pos) {
#ifdef CASIMIR_SAFE_CHECK
        if(pos >= length()) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot find the given position as it"
                                                       "doesn't correspond to any existing character");
        }
#endif
        return m_str.at(pos);
    }
    
    CASIMIR_EXPORT char utilities::String::at(const cuint &pos) const {
#ifdef CASIMIR_SAFE_CHECK
        if(pos >= length()) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot find the given position as it"
                                                       "doesn't correspond to any existing character");
        }
#endif
        return m_str.at(pos);
    }
    
    CASIMIR_EXPORT std::vector<utilities::String> utilities::String::split(const utilities::String& separator, bool discardEmptyStrings) const {
        // Create the list that will contains the output vector
        std::vector<String> sbStr;
        
        // If the length is null
        if(length() == 0) return {""};
        
        // The current position that has been processed
        cuint sPosition = 0;
        while (sPosition < length()) {
            // Compute the next position
            cuint nextPosition = std::min(findFirstOf(separator, sPosition), length());
            String nStr = substr(sPosition, nextPosition - sPosition);
            if (nStr.length() != 0 || !discardEmptyStrings) {
                sbStr.push_back(nStr);
            }

            // Increment the sPosition
            sPosition = nextPosition + separator.length();
        }
        
        // Return the resulting buffer
        return sbStr;
    }
    
    CASIMIR_EXPORT bool utilities::String::startsWith(const utilities::String &str) const {
        if(str.length() > length()) return false;
        for (cuint i = 0; i < str.length(); ++i) {
            if(str.at(i) != at(i))
                return false;
        }
        return true;
    }
    
    CASIMIR_EXPORT bool utilities::String::endsWith(const utilities::String &str) const {
        if(str.length() > length()) return false;
        for(cint i = length() - 1; i >= (cint) length() - str.length(); --i) {
            if(at(i) != str.at(i - length() + str.length()))
                return false;
        }
        return true;
    }
    
    CASIMIR_EXPORT utilities::String utilities::String::replaceAll(const utilities::String &str,
                                                                   const utilities::String &replacement) const {
        return replacement.join(split(str, false), false);
    }
    
    CASIMIR_EXPORT utilities::String utilities::String::join(std::vector<String> list, bool discardEmptyString) const {
        String output;
        for (cuint i = 0; i < list.size(); ++i) {
            if(i == 0 || (list[i] == "" && discardEmptyString)) {
                output += list[i];
            }
            else {
                output += literals::operator+((*this), list[i]);
            }
        }
        return output;
    }

    CASIMIR_EXPORT utilities::String utilities::String::encodeToHex() const {
        // Tools only used in this function that represent the hexadecimal alphabet
        static constexpr char hexAlphabet[] = {
                0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66
        };

        // Loop other each character of the string and replace each of the char by the hex correspondence
        // We already know the output length to be twice of the input length
        String output('\0', 2 * length());
        for(cuint i = 0; i < length(); ++i) {
            // Retrieve the character at position `i`
            const char value = at(i);

            // Convert the string to hexadecimal
            output[2 * i] = hexAlphabet[((value >> 4) & 0x0F)];
            output[2 * i + 1] = hexAlphabet[(value & 0x0F)];
        }

        // Return the resulting String
        return output;
    }

    CASIMIR_EXPORT utilities::String utilities::String::decodeFromHex() const {
        // We already know that the result will be half of the size of the current string
        if(length() % 2 != 0) {
            return String(); // The string isn't a valid string
        }

        // Otherwise allocate the result
        String result('\0', length() / 2);

        // Loop other each character of the resulting String
        for(cuint i = 0; i < length() / 2; ++i) {
            // Process separately v0 and v1
            const char _v0 = at(2 * i);
            char v0;
            if(_v0 >= 'a' && _v0 <= 'f') v0 = _v0 - 'a' + 10;
            else if(_v0 >= 'A' && _v0 <= 'F') v0 = _v0 - 'A' + 10;
            else if(_v0 >= '0' && _v0 <= '9') v0 = _v0 - '0';
            else return String(); // The current character isn't a valid hexadecimal character

            const char _v1 = at(2 * i + 1);
            char v1;
            if(_v1 >= 'a' && _v1 <= 'f') v1 = _v1 - 'a' + 10;
            else if(_v1 >= 'A' && _v1 <= 'F') v1 = _v1 - 'A' + 10;
            else if(_v1 >= '0' && _v1 <= '9') v1 = _v1 - '0';
            else return String(); // The current character isn't a valid hexadecimal character

            // Simply set the resulting byte
            result[i] = v0 << 4 | v1;
        }

        // Simply return the result
        return result;
    }

    CASIMIR_EXPORT utilities::String utilities::String::toUpperCase() const {
        String result(*this);
        for(cuint i = 0; i < length(); ++i) {
            char& value = result[i];
            if(value >= 'a' && value <= 'z') value -= 0x20;
        }
        return result;
    }

    CASIMIR_EXPORT utilities::String utilities::String::toLowerCase() const {
        String result(*this);
        for(cuint i = 0; i < length(); ++i) {
            char& value = result[i];
            if(value >= 'A' && value <= 'Z') value += 0x20;
        }
        return result;
    }

    CASIMIR_EXPORT void utilities::String::insert(const cuint &pos, const utilities::String &str) {
#ifdef CASIMIR_SAFE_CHECK
        if(pos > length()) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "The specified index to the index function isn't a valid index");
        }
#endif

        // Insert the str into the current instance
        m_str.insert(pos, str.c_str(), str.length());
    }

    CASIMIR_EXPORT utilities::String literals::operator+(const utilities::String& a, const utilities::String& b) {
        return utilities::String(a.str() + b.str());
    }
    
    
};

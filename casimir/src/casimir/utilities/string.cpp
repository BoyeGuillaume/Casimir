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
      if(start + length > this->length()) CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot perform the given operation"
                                                                                      "as the specified region isn't fully contained by the string");
#endif
        return String(m_str.substr(start, length));
    }
    
    CASIMIR_EXPORT char &utilities::String::at(const cuint &pos) {
#ifdef CASIMIR_SAFE_CHECK
        if(pos >= length()) CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot find the given position as it"
                                                                       "doesn't correspond to any existing character");
#endif
        return m_str.at(pos);
    }
    
    CASIMIR_EXPORT char utilities::String::at(const cuint &pos) const {
#ifdef CASIMIR_SAFE_CHECK
        if(pos >= length()) CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot find the given position as it"
                                                                       "doesn't correspond to any existing character");
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

    CASIMIR_EXPORT utilities::String literals::operator+(const utilities::String& a, const utilities::String& b) {
        return utilities::String(a.str() + b.str());
    }
    
    
};

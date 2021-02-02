#include "exception.hpp"

namespace Casimir {

using namespace literals;

    CASIMIR_EXPORT utilities::Exception::Exception(const utilities::String &cause, const utilities::String &file,
                                               const cuint &line)
        : Exception("UNKNOWN", cause, file, line)
    {}

    CASIMIR_EXPORT utilities::Exception::Exception(const String &error, const String &cause, const String &file,
                                               const cuint &line)
        : m_str("Error " + error + " [" + file + " @ " + String::toString(line) + "]: " + cause)
    {}
    
    
}

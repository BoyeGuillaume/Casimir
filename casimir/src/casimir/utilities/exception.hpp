#ifndef CASIMIR_EXCEPTION_HPP_
#define CASIMIR_EXCEPTION_HPP_

#include <exception>

#include "../casimir.hpp"
#include "string.hpp"
#include "string_serializable.hpp"

#define CASIMIR_THROW_EXCEPTION(error, cause) throw Exception(error,   \
                                                        cause,   \
                                                        Casimir::utilities::String(__FILE__), \
                                                        __LINE__)

namespace Casimir {
    namespace utilities {

        class Exception : public std::exception, public StringSerializable {
        private:
            String m_str;

        public:
            CASIMIR_EXPORT Exception(const String& cause, const String& file, const cuint& line);

            CASIMIR_EXPORT Exception(const String& error, const String& cause, const String& file, const cuint& line);

            String toString() const override {
                return m_str;
            }

            const char * what() const noexcept override {
                return toString().c_str();
            }
        };

    };
};

#endif //CASIMIR_EXCEPTION_HPP_

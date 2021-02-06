#ifndef CASIMIR_EXCEPTION_HPP_
#define CASIMIR_EXCEPTION_HPP_

#include <exception>

#include "../casimir.hpp"
#include "string_serializable.hpp"
#include "string.hpp"

#define CASIMIR_THROW_EXCEPTION(error, cause) throw Exception(error,   \
                                                        cause,   \
                                                        Casimir::utilities::String(__FILE__), \
                                                        __LINE__)

namespace Casimir {
    namespace utilities {

        class String;

        /**
         * @brief This class is the exception class used by the whole Casimir project
         */
        class Exception : public std::exception, public StringSerializable {
        private:
            String m_str;

        public:
            /**
             * @brief Default constructor of the exception class
             * @param cause The cause of the error
             * @param file The file where the error occurred
             * @param line The line where the exception is thrown
             */
            CASIMIR_EXPORT Exception(const String& cause, const String& file, const cuint& line);

            /**
             * @brief Constructor of the exception class
             * @param error Small \see String without space that hold the error name
             * @param cause The cause of the error
             * @param file The file where the error occurred
             * @param line The line where the exception is thrown
             */
            CASIMIR_EXPORT Exception(const String& error, const String& cause, const String& file, const cuint& line);

            /**
             * @brief Convert the \see Exception to \see String
             * @return A formatted \see String that describe the \see sException
             */
            inline String toString() const override {
                return m_str;
            }

            /**
             * @brief Convert the \see Exception to a C string
             * @return A formatted C string
             */
            const char * what() const noexcept override {
                return toString().c_str();
            }
        };

    };
};

#endif //CASIMIR_EXCEPTION_HPP_

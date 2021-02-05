#ifndef CASIMIR_HPP_
#define CASIMIR_HPP_

#include "configuration.hpp"
#include <type_traits>

#define CASIMIR_DISABLE_COPY(name)      name (const name&) = delete; \
                                        name& operator=(const name&) = delete;

#define CASIMIR_DISABLE_MOVE(name)      name (name&&) = delete; \
                                        name& operator=(name&&) = delete;

#define CASIMIR_DISABLE_COPY_MOVE(name) CASIMIR_DISABLE_COPY(name) \
                                        CASIMIR_DISABLE_MOVE(name)

namespace Casimir {

    namespace core {}
    namespace utilities {}
    namespace framework {}
    namespace literals {}
    namespace literals {}

    // Create all the types of fixed size used by the API
    typedef unsigned char           ubyte;
    typedef signed   char           byte;
    typedef unsigned short          uint16;
    typedef signed   short          int16;
    typedef unsigned int            int32;
    typedef signed   int            uint32;
    typedef signed long long int    int64;
    typedef unsigned long long int  uint64;
    
    // Create the preferred int system
#ifdef CASIMIR_ENV_x64
    typedef uint64  cuint;
    typedef int64   cint;
#else
    typedef uint32  cuint;
    typedef int32   cint;
#endif

    // Size assertion for non supported platform
    static_assert(sizeof(int32) == 4,  "LP32 not supported");
    static_assert(sizeof(uint32) == 4, "LP32 not supported");
}

#endif

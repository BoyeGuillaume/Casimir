#ifndef CASIMIR_CONFIGURATION_HPP_
#define CASIMIR_CONFIGURATION_HPP_

#define CASIMIR_VERSION_MAJOR 0
#define CASIMIR_VERSION_MINOR 0
#define CASIMIR_VERSION_PATCH 0
#define CASIMIR_VERSION       0.0.0
#define CASIMIR_HOMEPAGE      
#define CASIMIR_DESCRIPTION   Optimise an computational graph for GPU/CPU

/* #undef CASIMIR_BUILD_SHARED */

#ifdef CASIMIR_BUILD_SHARED
    #define CASIMIR_EXPORT __declspec( dllexport )
#else
    #define CASIMIR_EXPORT 
#endif

// Determining the platform preferred number of bytes
#if _WIN32 || _WIN64
   #if _WIN64
     #define CASIMIR_ENV_x64
  #else
    #define CASIMIR_ENV_x32
  #endif
#endif

#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define CASIMIR_ENV_x64
  #else
    #define CASIMIR_ENV_x32
  #endif
#endif

// Assert that at least one of the two environment as been detected
#if !defined(CASIMIR_ENV_x64) && !defined(CASIMIR_ENV_x32)
    #error "Cannot detect the environment (x32 / x64) of the current platform"
#endif 


#endif

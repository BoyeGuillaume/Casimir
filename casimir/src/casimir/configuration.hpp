#ifndef CASIMIR_CONFIGURATION_H_
#define CASIMIR_CONFIGURATION_H_

#define CASIMIR_VERSION_MAJOR     0
#define CASIMIR_VERSION_MINOR     0
#define CASIMIR_VERSION_PATCH     0
#define CASIMIR_VERSION           0.0.0
#define CASIMIR_HOMEPAGE          
#define CASIMIR_DESCRIPTION       Optimise an computational graph for GPU/CPU

/* #undef CASIMIR_BUILD_SHARED */
#define CASIMIR_SAFE_CHECK        true
#define CASIMIR_LITERAL_OPERATOR  ON

#ifndef CASIMIR_SAFE_CHECK
#if defined(_MSC_VER)
#pragma message ( "WARNING: Casimir was build without the option CASIMIR_SAFE_CHECK. We strongly advise you to re-enable this option as it make the Bug far more complex to understand without it" )
#else
#warning "WARNING: Casimir was build without the option CASIMIR_SAFE_CHECK. We strongly advise you to re-enable this option as it make the Bug far more complex to understand without it"
#endif
#endif

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

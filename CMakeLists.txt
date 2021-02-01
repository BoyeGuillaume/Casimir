cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Options available for the Casimir project
option(CASIMIR_TESTS        "Enable the Google tests" OFF)
option(CASIMIR_BUILD_SHARED "Enabke to build the casimir library as a Dynamic Library (DLL)" OFF)


# Global variable
set(CASIMIR_INSTALL_PATH     "${CMAKE_INSTALL_PREFIX}")
set(CASIMIR_INSTALL_BIN      "${CASIMIR_INSTALL_PATH}/bin")
set(CASIMIR_INSTALL_LIBS     "${CASIMIR_INSTALL_PATH}/lib")
set(CASIMIR_INSTALL_INCLUDE  "${CASIMIR_INSTALL_PATH}/include")
set(CASIMIR_INSTALL_ARCHIVE  "${CASIMIR_INSTALL_PATH}/archive")


# Setting up C++ standard
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)


# Retrieve the file by reading the file version
include(cmake/version.cmake)
loadVersion(".version" "CASIMIR")


# Create the CASIMIR Project
message("Setting up the CASIMIR project v${CASIMIR_VERSION}")

set(PROJECT_NAME "Casimir")
set(PROJECT_HOMEPAGE_URL "https://github.com/mredstone6/Casimir")
set(PROJECT_DESCRIPTION "Optimise an computational graph for GPU/CPU")

project(${PROJECT_NAME} 
	VERSION ${CASIMIR_VERSION} 
    DESCRIPTION ${PROEJCT_DESCRIPTION}
    )


# Adding the code subdirectory
add_subdirectory(casimir)


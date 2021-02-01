cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Set the included directory required to build the casimir project
set(CASIMIR_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(CASIMIR_SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")


# List all of the header aimed at being exported by the library
# Template: **/**/**.h  or  **/**/**.hpp
set(CASIMIR_EXPORTED_HEADER
    "casimir/configuration.hpp"
    "casimir/casimir.hpp"
) 


# List all of the source files being compiled by the library
# Template: ${RAVEN_SOURCE_DIRS}/**/**.cpp  or  ${RAVEN_SOURCE_DIRS}/**/**.cu
set(CASIMIR_SOURCE_FILES

)


# Configure the file input for the CASIMIR library
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/configuration.hpp.in" 
    "${CASIMIR_INCLUDE_DIRS}/casimir/configuration.hpp")


# Adding the CASIMIR library
if(CASIMIR_BUILD_SHARED)
    add_library(Casimir SHARED ${CASIMIR_SOURCE_FILES})
else()
    add_library(Casimir STATIC ${CASIMIR_SOURCE_FILES})
endif()


# Include the dependencies for the Casimir project
include_directories(TARGET Casimir PUBLIC ${RAVEN_INCLUDE_DIRS})


# Set the properties of the library
set_property(TARGET Casimir PROPERTY VERSION ${CASIMIR_VERSION})
set_property(TARGET Casimir PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
set_property(TARGET Casimir PROPERTY CXX_STANDARD_REQUIRED ${CMAKE_CXX_STANDARD_REQUIRED})
set_property(TARGET Casimir PROPERTY CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS})


# Adding the install target
install(
    TARGETS Casimir
    EXPORT Casimir
    LIBRARY  DESTINATION "${CASIMIR_INSTALL_LIBS}"      COMPONENT shlib
    ARCHIVE  DESTINATION "${CASIMIR_INSTALL_ARCHIVE}"   COMPONENT lib
    RUNTIME  DESTINATION "${CASIMIR_INSTALL_BIN}"       COMPONENT bin
)


# Install each of the exported header
foreach(TARGET_FILE_INSTALL ${CASIMIR_EXPORTED_HEADER})
    get_filename_component(INSTALL_SUFFIX ${TARGET_FILE_INSTALL} DIRECTORY)
    install(FILES		"${CASIMIR_INCLUDE_DIRS}/${TARGET_FILE_INSTALL}" 
			DESTINATION "${CASIMIR_INSTALL_INCLUDE}/${INSTALL_SUFFIX}")
endforeach()


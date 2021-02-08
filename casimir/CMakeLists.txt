cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Set the included directory required to build the casimir project
set(CASIMIR_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CASIMIR_SOURCE_DIRS  "${CMAKE_CURRENT_SOURCE_DIR}/src/casimir")

# List all of the header aimed at being exported by the library
# Template: **/**/**.h  or  **/**/**.hpp
set(CASIMIR_EXPORTED_HEADER
        "casimir/configuration.hpp"
        "casimir/casimir.hpp"
        "casimir/core/context.hpp"
        "casimir/core/heap_interface.hpp"
        "casimir/core/heap_allocator.hpp"
        "casimir/utilities/string.hpp"
        "casimir/utilities/string_serializable.hpp"
        "casimir/utilities/exception.hpp"
        "casimir/utilities/uuid.hpp"
        "casimir/utilities/logger.hpp"
        "casimir/utilities/optional.hpp"
        "casimir/utilities/cmutex.hpp"
        "casimir/framework/contextual_object.hpp"
        "casimir/framework/interface.hpp"
        "casimir/framework/datatype.hpp"
        "casimir/framework/allocator.hpp"
        "casimir/framework/indexable.hpp"
)

# List all of the other header used by the project but not exported by the library
# Template: **/**/**.h or **/**/**.hpp
set(CASIMIR_PRIVATE_HEADER
        "casimir/core/private-context.hpp"
)

# List all of the source files being compiled by the library
# Template: ${CASIMIR_SOURCE_DIRS}/**/**.cpp  or  ${CASIMIR_SOURCE_DIRS}/**/**.cu
set(CASIMIR_SOURCE_FILES
        "${CASIMIR_SOURCE_DIRS}/casimir.cpp"
        "${CASIMIR_SOURCE_DIRS}/core/context.cpp"
        "${CASIMIR_SOURCE_DIRS}/core/private-context.cpp"
        "${CASIMIR_SOURCE_DIRS}/core/heap_allocator.cpp"
        "${CASIMIR_SOURCE_DIRS}/core/heap_interface.cpp"
        "${CASIMIR_SOURCE_DIRS}/utilities/string.cpp"
        "${CASIMIR_SOURCE_DIRS}/utilities/exception.cpp"
        "${CASIMIR_SOURCE_DIRS}/utilities/uuid.cpp"
        "${CASIMIR_SOURCE_DIRS}/utilities/logger.cpp"
        "${CASIMIR_SOURCE_DIRS}/utilities/cmutex.cpp"
        "${CASIMIR_SOURCE_DIRS}/framework/contextual_object.cpp"
        "${CASIMIR_SOURCE_DIRS}/framework/allocator.cpp"
        "${CASIMIR_SOURCE_DIRS}/framework/datatype.cpp"
        "${CASIMIR_SOURCE_DIRS}/framework/indexable.cpp"
        "${CASIMIR_SOURCE_DIRS}/framework/interface.cpp"
)

# Retrieve all the headers to the expected format
set(CASIMIR_HEADERS "${CASIMIR_EXPORTED_HEADER}" "${CASIMIR_PRIVATE_HEADER}")
prepend(CASIMIR_HEADERS "${CASIMIR_INCLUDE_DIRS}/")
set(CASIMIR_INCLUDE_DIRS "${CASIMIR_INCLUDE_DIRS}" PARENT_SCOPE)

# Configure the file input for the CASIMIR library
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/configuration.hpp.in" "${CASIMIR_INCLUDE_DIRS}/casimir/configuration.hpp")

# Adding the CASIMIR library
if(CASIMIR_BUILD_SHARED)
    add_library(Casimir SHARED ${CASIMIR_SOURCE_FILES} ${CASIMIR_HEADERS})
else()
    add_library(Casimir STATIC ${CASIMIR_SOURCE_FILES} ${CASIMIR_HEADERS})
endif()

# Include the dependencies for tShe Casimir project
# target_include_directories(Casimir PUBLIC "${CASIMIR_INCLUDE_DIRS}")
# include_directories(SYSTEM ./src)

# Set the properties of the library
set_property(TARGET Casimir PROPERTY VERSION ${CASIMIR_VERSION})
set_property(TARGET Casimir PROPERTY SOVERSION ${CASIMIR_VERSION_MAJOR})
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

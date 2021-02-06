cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Configuration of the documentation generation
set(CASIMIR_DOXYGEN_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
set(CASIMIR_DOXYGEN_OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")

find_package(Doxygen)
if(DOXYGEN_FOUND)
    # Configure the Doxygen config file
    configure_file(${CASIMIR_DOXYGEN_INPUT_FILE} ${CASIMIR_DOXYGEN_OUTPUT_FILE} @ONLY)

    # Create the output directory
    file(MAKE_DIRECTORY ${CASIMIR_INSTALL_DOC})

    # Adding custom target for documentation generation
    add_custom_target(generate-doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${CASIMIR_DOXYGEN_OUTPUT_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else()
    message(ERROR "Cannot find package Doxygen required for building documentation")
endif()


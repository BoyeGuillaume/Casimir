cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Set up the Google test options
set(BUILD_GMOCK            OFF CACHE BOOL "Builds the googlemock subproject")
set(INSTALL_GTEST          OFF CACHE BOOL "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)")
set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")


# Add the googletest subdirectory (where all the magics happen)
add_subdirectory(googletest)

# Libraries to be included
set(CASIMIR_GTEST_REQUIRED_LIBS
        gtest
        gtest_main
        Casimir)

# Configure flags for the Google test
if(MSVC)
    set(CMAKE_CXX_FLAGS "/permissive- /std:c++17 ${CMAKE_CXX_FLAGS} /utf-8 /wd4530 /wd4577")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifestuac:NO /ignore:4099 /ignore:4098")
    add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused -Wno-unused-parameter")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

# Retrieve a list of all the test files
file(GLOB_RECURSE CASIMIR_FILE_TEST_LIST CONFIGURE_DEPENDS *_test.cpp)

# For each of the test define above
foreach(TEST_NAME ${CASIMIR_FILE_TEST_LIST})
    file(RELATIVE_PATH TEST_RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${TEST_NAME}")
    get_filename_component(EXEC_NAME_FILE ${TEST_NAME} NAME_WE)
    get_filename_component(EXEC_NAME_PATH ${TEST_RELATIVE_PATH} DIRECTORY)
    string(REPLACE "\\" "-" EXEC_NAME_PATH "${EXEC_NAME_PATH}")
    string(REPLACE "/" "-" EXEC_NAME_PATH "${EXEC_NAME_PATH}")
    string(LENGTH "${EXEC_NAME_PATH}" EXEC_NAME_PATH_LENGTH)

    if(${EXEC_NAME_PATH_LENGTH} EQUAL 0)
        set(EXEC_NAME "${EXEC_NAME_FILE}")
    else()
        set(EXEC_NAME "${EXEC_NAME_PATH}-${EXEC_NAME_FILE}")
    endif()
    message("Add test: ${EXEC_NAME}")

    add_executable(${EXEC_NAME} ${TEST_NAME})
    set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
    set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD_REQUIRED ${CMAKE_CXX_REQUIRED})
    set_property(TARGET ${EXEC_NAME} PROPERTY CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS})
    target_link_libraries(${EXEC_NAME} ${CASIMIR_GTEST_REQUIRED_LIBS})
    target_include_directories(${EXEC_NAME} PUBLIC ${CASIMIR_INCLUDE_DIRS})

    add_test(
            NAME ${EXEC_NAME}
            COMMAND $<TARGET_FILE:${EXEC_NAME}>
            WORKING_DIRECTORY ${CASIMIR_GTEST_WORKING_DIRECTORY}
    )
endforeach(TEST_NAME)

###########################
# Init
##########################
cmake_minimum_required(VERSION 3.5)
set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})

# Generates compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(concat LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)

if (MSVC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4 /utf-8")
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

add_subdirectory("lib/")

############
# Tests
############
option(UNIT_TEST "Build unit tests" OFF)
if (UNIT_TEST)
    enable_testing()
    add_subdirectory("test/")
endif()

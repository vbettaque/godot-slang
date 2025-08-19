# Using the same minimum as the godot-cpp project
cmake_minimum_required(VERSION 3.17)

# Silence unused variable warning when specified from toolchain
if(CMAKE_C_COMPILER)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)

set(LIBNAME "godot-slang" CACHE STRING "The name of the library")

set(GODOT_CPP_DIR "external/godot-cpp" CACHE STRING "The directory of the godot-cpp submodule")
set(SLANG_DIR "external/slang" CACHE STRING "The directory of the slang submodule")

# Make sure all the dependencies are satisfied
find_package(Python3 3.4 REQUIRED)
find_program(GIT git REQUIRED)

# Ensure godot-cpp submodule has been updated
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${GODOT_CPP_DIR}/src")
    message(NOTICE "godot-cpp bindings source not found")
    message(NOTICE "initializing/updating the godot-cpp submodule...")

    # update the c++ bindings submodule to populate it with the necessary source for the library
    execute_process(
        COMMAND git submodule update --init godot-cpp
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${GODOT_CPP_DIR}"
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# Ensure slang submodule has been updated
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${SLANG_DIR}/source")
    message(NOTICE "slang bindings source not found")
    message(NOTICE "initializing/updating the slang submodule...")

    # update the c++ bindings submodule to populate it with the necessary source for the library
    execute_process(
        COMMAND git submodule update --init --recursive slang
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${SLANG_DIR}"
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

add_subdirectory(${GODOT_CPP_DIR} "godot-cpp" SYSTEM)
add_subdirectory(${SLANG_DIR} "slang" SYSTEM)

# Add godot-cpp's module path and include the exported functions.
# This is made available for documentation generation
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${godot-cpp_SOURCE_DIR}/cmake")
include(GodotCPPModule)

# The godot-cpp target has some of useful properties attached that can be retrieved like so.
get_target_property(GODOTCPP_SUFFIX godot::cpp GODOTCPP_SUFFIX)
get_target_property(GODOTCPP_PLATFORM godot::cpp GODOTCPP_PLATFORM)

# Now we can specify our own project which will inherit any global cmake properties or variables that have been defined.
project(${LIBNAME}
    VERSION 0.1
    DESCRIPTION "Slang shader language bindings for Godot."
    HOMEPAGE_URL "https://github.com/vbettaque/godot-slang"
    LANGUAGES CXX
)

add_library(${LIBNAME} SHARED)

target_sources(${LIBNAME}
    PRIVATE
    src/register_types.cpp
    src/register_types.h
    src/slang_session.cpp
    src/slang_session.h
    src/slang_program.h
    src/slang_module.h
)

# Fetch a list of the xml files to use for documentation and add to our target
file(GLOB_RECURSE DOC_XML LIST_DIRECTORIES NO CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/docs/*.xml")

# conditionally add doc data to compile output
if(DOC_XML)
    if(GODOTCPP_TARGET MATCHES "editor|template_debug")
        target_doc_sources(${LIBNAME} ${DOC_XML})
    endif()
endif()

target_link_libraries(${LIBNAME} PRIVATE godot-cpp)
target_link_libraries(${LIBNAME} PRIVATE slang)

set_target_properties(${LIBNAME}
    PROPERTIES
    # The generator expression here prevents msvc from adding a Debug or Release subdir.
    LIBRARY_OUTPUT_DIRECTORY "$<1:${PROJECT_SOURCE_DIR}/bin/${GODOTCPP_PLATFORM}>"
    OUTPUT_NAME "${LIBNAME}${GODOTCPP_SUFFIX}"
    BUILD_WITH_INSTALL_RPATH FALSE
    LINK_FLAGS "-Wl,-rpath,$ORIGIN/"
)

set_target_properties(slang
    PROPERTIES
    # The generator expression here prevents msvc from adding a Debug or Release subdir.
    LIBRARY_OUTPUT_DIRECTORY "$<1:${PROJECT_SOURCE_DIR}/bin/${GODOTCPP_PLATFORM}>"
    OUTPUT_NAME "slang${GODOTCPP_SUFFIX}"
)

set(GODOT_PROJECT_DIR "demo" CACHE STRING "The directory of a Godot project folder")
set(GODOT_PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/${GODOT_PROJECT_DIR}/addons/${LIBNAME}/bin/${GODOTCPP_PLATFORM}")

add_custom_command(TARGET ${LIBNAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${LIBNAME}>" "${GODOT_PROJECT_BINARY_DIR}/$<TARGET_FILE_NAME:${LIBNAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:slang>" "${GODOT_PROJECT_BINARY_DIR}/$<TARGET_FILE_NAME:slang>"
)
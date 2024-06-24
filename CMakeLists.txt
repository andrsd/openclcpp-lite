cmake_minimum_required(VERSION VERSION 3.16)

project(openclcpp-lite VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(FetchContent)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CodeCoverage)

option(OPENCLCPP_LITE_BUILD_TESTS "Build tests" NO)

find_package(OpenCL REQUIRED)
find_package(fmt 8.0 REQUIRED)

if (OPENCLCPP_LITE_BUILD_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    mark_as_advanced(FORCE
        BUILD_GMOCK
        INSTALL_GTEST
        FETCHCONTENT_SOURCE_DIR_GOOGLETEST
        FETCHCONTENT_UPDATES_DISCONNECTED_GOOGLETEST
    )

    FetchContent_MakeAvailable(googletest)
    mark_as_advanced(FORCE
        FETCHCONTENT_BASE_DIR
        FETCHCONTENT_FULLY_DISCONNECTED
        FETCHCONTENT_QUIET
        FETCHCONTENT_UPDATES_DISCONNECTED
    )
endif ()

add_subdirectory(src)

# Binaries

add_subdirectory(bin)

# Tests

if (OPENCLCPP_LITE_BUILD_TESTS)
    set(OPENCLCPP_LITE_UNIT_TESTS_ROOT ${CMAKE_SOURCE_DIR}/tests CACHE PATH "" FORCE)
    mark_as_advanced(FORCE OPENCLCPP_LITE_UNIT_TESTS_ROOT)

    enable_testing()
    add_subdirectory(tests)
endif()

# Install

install(
    FILES
        "${PROJECT_SOURCE_DIR}/cmake/OpenCLProgram.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/openclcpp-lite
)

install(
    FILES
        "${PROJECT_SOURCE_DIR}/python/program.h.jinja2"
        "${PROJECT_SOURCE_DIR}/python/program.cpp.jinja2"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/openclcpp-lite/templates
)

install(
    PROGRAMS
        "${PROJECT_SOURCE_DIR}/python/oclcppl-gen.py"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

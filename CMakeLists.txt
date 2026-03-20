cmake_minimum_required(VERSION 3.23)

project(openclcpp-lite
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CodeCoverage)

option(OPENCLCPP_LITE_BUILD_TESTS "Build tests" NO)

find_package(OpenCL REQUIRED)
find_package(fmt 11.0 REQUIRED)
find_package(jinja2 REQUIRED)
find_package(pyyaml REQUIRED)

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
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/OpenCLProgram.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/openclcpp-lite
)

install(
    FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/python/program.h.jinja2"
        "${CMAKE_CURRENT_SOURCE_DIR}/python/program.cpp.jinja2"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/openclcpp-lite/templates
)

install(
    PROGRAMS
        "${CMAKE_CURRENT_SOURCE_DIR}/python/oclcppl-gen.py"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

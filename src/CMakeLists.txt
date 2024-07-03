add_library(openclcpp-lite
    SHARED
        buffer.cpp
        context.cpp
        device.cpp
        enums.cpp
        error.cpp
        event.cpp
        exception.cpp
        kernel.cpp
        memory.cpp
        platform.cpp
        program.cpp
        queue.cpp
        template.cpp
        utils.cpp
)

target_code_coverage(openclcpp-lite)

target_include_directories(
    openclcpp-lite
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/openclcpp-lite>
)

target_link_libraries(
    openclcpp-lite
    PUBLIC
        OpenCL::OpenCL
)

if (${fmt_FOUND})
    target_compile_definitions(openclcpp-lite PUBLIC OPENCLCPP_LITE_WITH_FMT)
    target_link_libraries(openclcpp-lite PUBLIC fmt::fmt)
endif()

# Install

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/openclcpp-lite-config.cmake.in
    openclcpp-lite-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/openclcpp-lite
    NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file(
    openclcpp-lite-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    TARGETS openclcpp-lite
    EXPORT OpenCLCppLiteTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(
    EXPORT OpenCLCppLiteTargets
    FILE openclcpp-lite-targets.cmake
    NAMESPACE openclcpp-lite::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/openclcpp-lite
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/openclcpp-lite-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/openclcpp-lite-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/openclcpp-lite
)

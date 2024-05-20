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

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/openclcpp-lite
    FILES_MATCHING PATTERN "*.h"
)

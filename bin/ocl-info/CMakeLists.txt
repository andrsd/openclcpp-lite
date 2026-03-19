project(ocl-info)

add_executable(ocl-info)

target_sources(
    ocl-info
    PRIVATE
        main.cpp
)

target_include_directories(
    ocl-info
    PRIVATE
        ${CMAKE_SOURCE_DIR}/contrib
)

target_link_libraries(
    ocl-info
    PRIVATE
        openclcpp-lite
)

target_compile_features(ocl-info PUBLIC cxx_std_20)

install(
    TARGETS ocl-info
    EXPORT openclcpp-lite-targets
    RUNTIME DESTINATION bin
)

project(ocl-linker)

add_executable(ocl-linker)

target_sources(
    ocl-linker
    PRIVATE
        main.cpp
)

target_include_directories(
    ocl-linker
    PRIVATE
        ${CMAKE_SOURCE_DIR}/contrib
)

target_link_libraries(
    ocl-linker
    PRIVATE
        openclcpp-lite
        fmt::fmt
)

target_compile_features(ocl-linker PUBLIC cxx_std_20)

install(
    TARGETS ocl-linker
    EXPORT openclcpp-lite-targets
    RUNTIME DESTINATION bin
)

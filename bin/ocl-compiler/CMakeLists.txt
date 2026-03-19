project(ocl-compiler)

add_executable(ocl-compiler)

target_sources(
    ocl-compiler
    PRIVATE
        main.cpp
)

target_include_directories(
    ocl-compiler
    PRIVATE
        ${CMAKE_SOURCE_DIR}/contrib
)

target_link_libraries(
    ocl-compiler
    PRIVATE
        openclcpp-lite
)

target_compile_features(ocl-compiler PUBLIC cxx_std_20)

install(
    TARGETS ocl-compiler
    EXPORT openclcpp-lite-targets
    RUNTIME DESTINATION bin
)

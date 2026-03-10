project(ocl-linker)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/contrib
)

target_link_libraries(${PROJECT_NAME} PRIVATE openclcpp-lite)

target_compile_features(${PROJECT_NAME} public cxx_std_17)

install(
    TARGETS ocl-linker
    EXPORT OpenCLCppLiteTargets
    RUNTIME DESTINATION bin
)

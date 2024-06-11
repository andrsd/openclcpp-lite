project(oclc)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/contrib
)

target_link_libraries(${PROJECT_NAME} PRIVATE openclcpp-lite)

install(
    TARGETS oclc
    EXPORT OpenCLCppLiteTargets
    RUNTIME DESTINATION bin
)

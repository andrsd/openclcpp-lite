project(openclcpp-lite-test)

include(GoogleTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    main.cpp
    Exception_test.cpp
    Platform_test.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/openclcpp-lite>
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        gtest_main
        gmock_main
        OpenCL::OpenCL
)

if (${fmt_FOUND})
    target_compile_definitions(${PROJECT_NAME} PUBLIC OPENCLCPP_LITE_WITH_FMT)
    target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
endif()

target_code_coverage(${PROJECT_NAME})

add_test(
    NAME openclcpp-lite-test
    COMMAND openclcpp-lite-test
)

if(OPENCLCPP_LITE_CODE_COVERAGE)
    set_tests_properties(
        openclcpp-lite-test
        PROPERTIES
            ENVIRONMENT LLVM_PROFILE_FILE=openclcpp-lite-test-1.profraw
    )
endif()

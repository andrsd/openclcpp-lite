project(openclcpp-lite-test)

include(GoogleTest)
include(OpenCLProgram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    main.cpp
    Buffer_test.cpp
    Context_test.cpp
    Error_test.cpp
    Event_test.cpp
    Exception_test.cpp
    Kernel_test.cpp
    KernelFunctor_test.cpp
    Range_test.cpp
    Platform_test.cpp
    Program_test.cpp
    Queue_test.cpp
)

if (CI_ENABLED)
    target_compile_definitions(${PROJECT_NAME} PRIVATE CI_ENABLED)
else()
    target_sources(${PROJECT_NAME} PRIVATE TestVectorProgram_test.cpp)

    target_add_opencl_program(${PROJECT_NAME}
        CLASS TestVectorProgram
        FILE test_vector_program.cl
    )
endif()


target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include/openclcpp-lite>
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        gtest_main
        gmock_main
        openclcpp-lite
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
            ENVIRONMENT LLVM_PROFILE_FILE=openclcpp-lite-test.profraw
    )
endif()

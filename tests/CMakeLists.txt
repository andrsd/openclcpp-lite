# openclcpp-lite-test

find_package(GTest)

include(OpenCLProgram)

add_executable(openclcpp-lite-test)

target_sources(
    openclcpp-lite-test
    PRIVATE
        main.cpp
        Atomics_test.cpp
        Buffer_test.cpp
        Context_test.cpp
        Device_test.cpp
        Error_test.cpp
        Event_test.cpp
        Exception_test.cpp
        Kernel_test.cpp
        KernelFunctor_test.cpp
        Range_test.cpp
        Platform_test.cpp
        Program_test.cpp
        Queue_test.cpp
        Template_test.cpp
        Utils_test.cpp
)

if (CI_ENABLED)
    target_compile_definitions(openclcpp-lite-test PRIVATE CI_ENABLED)
else()
    target_sources(openclcpp-lite-test PRIVATE TestVectorProgram_test.cpp)

    target_add_opencl_program(openclcpp-lite-test
        CLASS TestVectorProgram
        FILE test_vector_program.cl
        HEADER test_vector_program.cl.h
        SOURCE test_vector_program.cl.cpp
        INCLUDE test_vector_program.cl.h
    )
endif()


# target_include_directories(
#     openclcpp-lite-test
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
#         $<INSTALL_INTERFACE:include/openclcpp-lite>
# )

target_link_libraries(
    openclcpp-lite-test
    PUBLIC
        openclcpp-lite
        GTest::gmock_main
)

target_compile_features(openclcpp-lite-test public cxx_std_17)

if (${fmt_FOUND})
    target_compile_definitions(openclcpp-lite-test PUBLIC OPENCLCPP_LITE_WITH_FMT)
    target_link_libraries(openclcpp-lite-test PUBLIC fmt::fmt)
endif()

target_code_coverage(openclcpp-lite-test)

add_test(
    NAME openclcpp-lite-test
    COMMAND openclcpp-lite-test
)

if (OPENCLCPP_LITE_CODE_COVERAGE)
    set_tests_properties(
        openclcpp-lite-test
        PROPERTIES
            ENVIRONMENT LLVM_PROFILE_FILE=openclcpp-lite-test.profraw
    )
endif()

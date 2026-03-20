include(CMakeParseArguments)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import yaml; print(yaml.__version__)"
    OUTPUT_VARIABLE TMP
    ERROR_QUIET
    RESULT_VARIABLE RES
)
if (RES EQUAL 0)
    string(REGEX MATCH "([0-9]+\.[0-9]+\.[0-9]+)" PYYAML_VERSION ${TMP})
    set(PYYAML_VERSION ${CMAKE_MATCH_1})
    message(STATUS "Found pyyaml: version \"${PYYAML_VERSION}\"")
    set(PYYAML_FOUND TRUE)
else()
    message(FATAL_ERROR "pyyaml was not found")
    set(PYYAML_FOUND FALSE)
endif()

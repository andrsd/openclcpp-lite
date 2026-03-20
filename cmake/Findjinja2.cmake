include(CMakeParseArguments)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import jinja2; print(jinja2.__version__)"
    OUTPUT_VARIABLE TMP
    ERROR_QUIET
    RESULT_VARIABLE RES
)
if (RES EQUAL 0)
    string(REGEX MATCH "([0-9]+\.[0-9]+\.[0-9]+)" JINJA2_VERSION ${TMP})
    set(JINJA2_VERSION ${CMAKE_MATCH_1})
    message(STATUS "Found jinja2: version \"${JINJA2_VERSION}\"")
    set(JINJA2_FOUND TRUE)
else()
    message(FATAL_ERROR "jinja2 was not found")
    set(JINJA2_FOUND FALSE)
endif()

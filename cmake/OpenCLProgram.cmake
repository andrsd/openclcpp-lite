# Module for compiling OpenCL code as a `Program`-based class
#
# USAGE:
# target_add_opencl_program(<target>
#    CLASS <C++ class name>
#    FILE <file with OpenCL source code>
# )

if(CMAKE_PROJECT_NAME STREQUAL "openclcpp-lite")
    set(OCLC ocl-compiler)
    set(OCLCPPL_GEN ${CMAKE_SOURCE_DIR}/python/oclcppl-gen.py)
    set(OCLCPPL_LOC ${CMAKE_SOURCE_DIR}/python)
else()
    find_program(OCLC ocl-compiler)
    if (${OCLC} STREQUAL OCLC-NOTFOUND)
        message(FATAL_ERROR "ocl-compiler not found")
    endif()

    find_program(OCLCPPL_GEN oclcppl-gen.py)
    if (${OCLCPPL_GEN} STREQUAL OCLCPPL_GEN-NOTFOUND)
        message(FATAL_ERROR "oclcppl-gen.py not found")
    endif()

    find_path(OCLCPPL_LOC program.cpp.jinja2
        PATH_SUFFIXES
            share/openclcpp-lite/templates
    )
    if (${OCLCPPL_LOC} STREQUAL OCLCPPL_LOC-NOTFOUND)
        message(FATAL_ERROR "openclcpp-lite template loction not found")
    endif()
endif()

function(target_add_opencl_program TARGET)
    cmake_parse_arguments(OCLCPPL "" "CLASS;FILE;HEADER;SOURCE;INCLUDE;NAMESPACE" "DEPENDS" ${ARGN})

    if (NOT IS_ABSOLUTE ${OCLCPPL_FILE})
        set(OCLCPPL_HEADER ${PROJECT_BINARY_DIR}/${OCLCPPL_HEADER})
        set(OCLCPPL_SOURCE ${PROJECT_BINARY_DIR}/${OCLCPPL_SOURCE})
    endif()

    add_custom_command(
        OUTPUT
            ${OCLCPPL_HEADER}
            ${OCLCPPL_SOURCE}
        COMMAND
            ${OCLC} ${CMAKE_CURRENT_SOURCE_DIR}/${OCLCPPL_FILE}
        COMMAND
            ${Python3_EXECUTABLE} ${OCLCPPL_GEN}
            -t program.h.jinja2
            -l ${OCLCPPL_LOC}
            namespace="${OCLCPPL_NAMESPACE}"
            class_name="${OCLCPPL_CLASS}" > ${OCLCPPL_HEADER}
        COMMAND
            ${Python3_EXECUTABLE} ${OCLCPPL_GEN}
            -t program.cpp.jinja2
            -l ${OCLCPPL_LOC}
            -s ${CMAKE_CURRENT_SOURCE_DIR}/${OCLCPPL_FILE}
            namespace="${OCLCPPL_NAMESPACE}"
            class_name="${OCLCPPL_CLASS}"
            header_file="${OCLCPPL_INCLUDE}" > ${OCLCPPL_SOURCE}
        DEPENDS
            ${OCLCPPL_FILE}
            ${OCLCPPL_DEPENDS}
        COMMENT
            "Generating code for ${OCLCPPL_CLASS} class"
    )

    target_sources(${TARGET} PRIVATE ${OCLCPPL_HEADER} ${OCLCPPL_SOURCE})
endfunction()

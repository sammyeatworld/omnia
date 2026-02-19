include(CMakeParseArguments)
include(GenerateExportHeader)

function(omnia_lib target)
    cmake_parse_arguments(OMNIA "" "" "SOURCES" ${ARGN})

    add_library(${target})

    target_sources(${target} PRIVATE ${OMNIA_SOURCES})
    target_link_libraries(${target} PUBLIC Common)

    string(REGEX REPLACE "^Lib" "" short_name ${target})
    string(TOUPPER ${short_name} short_name_upper)

    set(export_dir "${CMAKE_CURRENT_BINARY_DIR}/include/${target}")
    file(MAKE_DIRECTORY "${export_dir}")

    generate_export_header(${target}
            EXPORT_MACRO_NAME ${short_name_upper}_API
            EXPORT_FILE_NAME "${export_dir}/Export.h"
    )

    target_include_directories(${target} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    )
endfunction()

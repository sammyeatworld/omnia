include(CMakeParseArguments)

function(omnia_lib target)
    cmake_parse_arguments(OMNIA "" "" "SOURCES;PUBLIC_HEADERS" ${ARGN})

    add_library(${target})

    target_sources(${target} PRIVATE ${OMNIA_SOURCES})
    target_link_libraries(${target} PUBLIC Common)
endfunction()

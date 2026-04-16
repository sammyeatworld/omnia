add_library(LibMath INTERFACE)

target_include_directories(LibMath INTERFACE
        $<BUILD_INTERFACE:${OMNIA_SOURCE_DIR}/Libraries>
)

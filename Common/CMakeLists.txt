add_library(Common INTERFACE)

target_include_directories(Common INTERFACE
        $<BUILD_INTERFACE:${OMNIA_SOURCE_DIR}>
        $<BUILD_INTERFACE:${OMNIA_SOURCE_DIR}/Libraries>
)

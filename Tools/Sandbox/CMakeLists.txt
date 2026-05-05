set(SOURCES
        Main.cpp
)

omnia_app(Sandbox
        SOURCES ${SOURCES}
        LIBS LibPlatform LibRHI LibAsset LibRenderer
)
add_dependencies(Sandbox CopyResources)
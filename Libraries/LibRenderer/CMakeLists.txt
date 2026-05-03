set(SOURCES
        Camera.cpp
        Material.cpp
        Model.cpp
        SubMesh.cpp
)

omnia_lib(LibRenderer
        SOURCES ${SOURCES}
        LIBS LibRHI
)

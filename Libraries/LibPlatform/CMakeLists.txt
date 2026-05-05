set(SOURCES
        Input.cpp
)

if (WIN32)
    list(APPEND SOURCES
            Win32/WindowWin32.cpp
    )
endif ()

omnia_lib(LibPlatform
        SOURCES ${SOURCES}
)

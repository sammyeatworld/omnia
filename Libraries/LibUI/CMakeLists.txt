set(SOURCES
        Platform/Event.cpp
        Platform/Input.cpp
)

if (WIN32)
    list(APPEND SOURCES
            Platform/Win32/WindowWin32.cpp
    )
endif ()

omnia_lib(LibUI
        SOURCES ${SOURCES}
)

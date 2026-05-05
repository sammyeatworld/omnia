set(SOURCES
    Vec2.test.cpp
    Vec3.test.cpp
    Vec4.test.cpp
    Mat4.test.cpp
    Quat.test.cpp
)

omnia_test(LibMath
    SOURCES ${SOURCES}
    LIBS LibMath
)
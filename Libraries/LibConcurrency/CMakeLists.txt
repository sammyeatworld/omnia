set(SOURCES
        Scheduler.cpp
        ThreadPool.cpp
)

omnia_lib(LibConcurrency
        SOURCES ${SOURCES}
)

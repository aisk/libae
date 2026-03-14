cmake_minimum_required(VERSION 3.16)

project(libae VERSION 7.2 LANGUAGES C)

include(GNUInstallDirs)

option(AE_BUILD_EXAMPLES "Build bundled libae example programs" ON)

set(AE_WARNINGS
    -Wno-parentheses
    -Wno-switch-enum
    -Wno-unused-value
)

set(AE_PUBLIC_HEADERS
    src/ae.h
    src/anet.h
    src/monotonic.h
)

set(AE_PRIVATE_HEADERS
    src/config.h
    src/fmacros.h
    src/zmalloc.h
)

add_library(ae STATIC
    src/ae.c
    src/anet.c
    src/monotonic.c
    ${AE_PUBLIC_HEADERS}
    ${AE_PRIVATE_HEADERS}
)

add_library(ae::ae ALIAS ae)

set_target_properties(ae PROPERTIES
    OUTPUT_NAME ae
    PUBLIC_HEADER "${AE_PUBLIC_HEADERS}"
)

target_include_directories(ae
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_options(ae PRIVATE ${AE_WARNINGS})

if(AE_BUILD_EXAMPLES)
    add_executable(timer example/timer.c)
    target_link_libraries(timer PRIVATE ae)
    target_compile_options(timer PRIVATE ${AE_WARNINGS})

    add_executable(echo example/echo.c)
    target_link_libraries(echo PRIVATE ae)
    target_compile_options(echo PRIVATE ${AE_WARNINGS})
endif()

install(TARGETS ae
    EXPORT aeTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT aeTargets
    FILE aeTargets.cmake
    NAMESPACE ae::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ae
)

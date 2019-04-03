
add_executable(
    FixedPointVectorExample
    main.cpp
)

target_include_directories(
    FixedPointVectorExample
    PUBLIC
)

set_target_properties(
    FixedPointVectorExample
    PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(
    FixedPointVectorExample
    FixedPointVector
)


# Installation

install(
    TARGETS FixedPointVectorExample
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

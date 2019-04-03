
add_library(
    FixedPointVector
    INTERFACE
)

target_include_directories(
    FixedPointVector
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# Installation

install(
    TARGETS FixedPointVector EXPORT FixedPointVectorConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}  # This is for Windows
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into 'cmake'.
install(EXPORT FixedPointVectorConfig DESTINATION share/fixedpointvector/cmake)

# This makes the project importable from the build directory
export(TARGETS FixedPointVector FILE FixedPointVectorConfig.cmake)


# Testing

if(CMAKE_TESTING_ENABLED)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)

    add_library(boost-test SHARED IMPORTED)
    set_property(TARGET boost-test PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
    set_property(TARGET boost-test PROPERTY IMPORTED_LOCATION ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})


    add_executable(
        FixedPointVectorTest
        test/main.cpp
        test/fixedpointvector-test.cpp
    )

    set_target_properties(
        FixedPointVectorTest
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    target_link_libraries(
        FixedPointVectorTest
        boost-test
        FixedPointVector
    )

    add_test(FixedPointVectorTest FixedPointVectorTest)
endif(CMAKE_TESTING_ENABLED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(SYSTEM $ENV{GOOGLETEST_INCLUDE_DIR})


file(GLOB_RECURSE TEST_FILES "*.cpp")

link_directories($ENV{GOOGLETEST_LIB_DIR})

add_executable(turbodbc_test ${TEST_FILES})
add_dependencies(turbodbc_test googletest_build)

add_dependencies(turbodbc_test
    refresh_cmake_configuration
)

target_link_libraries(turbodbc_test
    turbodbc
    cpp_odbc
    ${Odbc_LIBRARIES}
    ${PYTHON_LIBRARIES}
    gmock_main
    gtest
    gmock
)

IF(UNIX)
target_link_libraries(turbodbc_test
    pthread
)
ENDIF()

add_test(NAME turbodbc_unit_test
         COMMAND turbodbc_test --gtest_output=xml:${CMAKE_BINARY_DIR}/turbodbc_test.xml)

IF(WIN32)
set_tests_properties(
    turbodbc_unit_test
    PROPERTIES
    ENVIRONMENT "PATH=${CMAKE_BINARY_DIR};$ENV{PATH}"
)
ENDIF()

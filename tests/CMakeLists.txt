option(test "Build all tests." OFF)


if (test)

set(TEST_NAME "TapDevStackTest")

set(TEST_SRC ${PROJECT_SOURCE_DIR}/tests/src)

enable_testing()

add_executable(${TEST_NAME}
${TEST_SRC}/test.cc
)

target_include_directories(${TEST_NAME} PUBLIC ${STACK_DIR})

target_link_directories(${TEST_NAME} PUBLIC ${STACK_DIR})

target_link_libraries(${TEST_NAME} PUBLIC GTest::gtest_main PUBLIC stack_lib)


endif()

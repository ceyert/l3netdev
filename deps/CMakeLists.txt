set(DEP_SRC_DIR ${CMAKE_BINARY_DIR}/dependency_sources)
file(MAKE_DIRECTORY ${DEP_SRC_DIR})

include(FetchContent)

if (NOT TARGET googletest)

set(GTEST_SRC_DIR ${DEP_SRC_DIR}/googletest)
FetchContent_Declare(googletest
GIT_REPOSITORY      https://github.com/google/googletest.git
SOURCE_DIR          "${GTEST_SRC_DIR}"
GIT_TAG             release-1.12.1
)

endif()

add_subdirectory(gtest)
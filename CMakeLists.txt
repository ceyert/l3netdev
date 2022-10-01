cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

add_compile_options(-std=c++20 -Wall)

set(DEFAULT_BUILD_TYPE "Release")
if( (EXISTS "${CMAKE_SOURCE_DIR}/.git") OR (EXISTS "${CMAKE_SOURCE_DIR}/.vscode") )
  set(DEFAULT_BUILD_TYPE "Debug")
endif()


set(APP_NAME "TapDevStack")

project(${APP_NAME})

#*******************************************************
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(STACK_DIR ${LIB_DIR}/stack)
set(STACK_SRC ${STACK_DIR}/src)

set(MODS_DIR ${PROJECT_SOURCE_DIR}/mods)

set(NETDEV_DIR ${MODS_DIR}/netdev)
set(NETDEV_SRC ${NETDEV_DIR}/src)

set(PROCCESSOR_DIR ${MODS_DIR}/processor)
set(PROCCESSOR_SRC ${PROCCESSOR_DIR}/src)

#*******************************************************

add_subdirectory(lib)
add_subdirectory(mods)
add_subdirectory(app)
add_subdirectory(deps)
add_subdirectory(tests)

#*******************************************************
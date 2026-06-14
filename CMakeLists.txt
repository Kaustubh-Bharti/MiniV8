cmake_minimum_required(VERSION 3.15)

project(MiniV8)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/*.cpp"
)

list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")

add_executable(MiniV8 ${SOURCES})
cmake_minimum_required(VERSION 3.11)
set(PROJ_NAME GeckoEngine)
project(${PROJ_NAME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
include_directories(includes)
file(GLOB SOURCES src/*.cpp)

add_executable(${PROJ_NAME} ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJ_NAME} Threads::Threads)
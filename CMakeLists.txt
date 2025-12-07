cmake_minimum_required(VERSION 3.10)

project(AmazonsBot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURSES "src/*.cpp")

add_executable(main ${SOURSES})

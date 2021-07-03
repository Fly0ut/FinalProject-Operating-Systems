cmake_minimum_required(VERSION 3.19)
project(FinalProject)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(FinalProject src/main.cpp src/shell_wrap.cpp src/pageFaultSim.cpp src/commands.cpp)
cmake_minimum_required(VERSION 3.19)
project(FinalProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#QT packages needed
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

#QT setup
set(QT_VERSION 5)
set(REQUIRED_LIBS Core)
set(REQUIRED_LIBS_QUALIFIED QT5::CORE)

#Includes
include_directories(${PROJECT_SOURCE_DIR}/include)

#Sources
add_executable(FinalProject src/main.cpp src/shell_wrap.cpp src/pageFaultSim.cpp src/commands.cpp qtTemplates/PageFaultDisplay.ui qtTemplates/PageFaultDisplay.cpp src/pageAlgorithms.cpp)
#add_executable(FinalProject src/main.cpp src/shell_wrap.cpp src/pageFaultSim.cpp src/commands.cpp src/pageAlgorithms.cpp)

#QT links
target_link_libraries(FinalProject Qt5::Core)
target_link_libraries(FinalProject Qt5::Widgets)
target_link_libraries(FinalProject Qt5::Quick)
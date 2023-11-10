cmake_minimum_required(VERSION 3.26)
project(pixelSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(pixelSim main.cpp
        cmake-build-debug/GameObject.cpp
        cmake-build-debug/GameObject.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Users/jacob/Downloads/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.0/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/Users/jacob/Downloads/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.0/include)
target_link_libraries(pixelSim sfml-system sfml-window  sfml-graphics sfml-audio)

cmake_minimum_required(VERSION 3.26)
project(pixelSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(pixelSim main.cpp
        GuiClass.cpp
        GuiClass.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Users/Owner/Documents/Ai/PixelEngine/SFML-2.6.0/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(SFML-2.6.0/include)
target_link_libraries(pixelSim sfml-system sfml-window  sfml-graphics sfml-audio)


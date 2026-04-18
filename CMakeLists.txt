cmake_minimum_required(VERSION 3.15)
project(DSAVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(DSAVisualizer
    main.cpp
    visualizer/SortingVisualizer.cpp
    visualizer/GraphVisualizer.cpp
    algorithms/sorting_algorithms.cpp
    algorithms/graph_algorithms.cpp
    ui/Button.cpp
    ui/Slider.cpp
    utils/helpers.cpp
)

target_link_libraries(DSAVisualizer PRIVATE sfml-graphics sfml-window sfml-system)

if(MSVC)
    target_compile_options(DSAVisualizer PRIVATE /W4)
else()
    target_compile_options(DSAVisualizer PRIVATE -Wall -Wextra -Wpedantic)
endif()

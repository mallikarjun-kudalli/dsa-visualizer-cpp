# DSA Visualizer (C++ + SFML)

Interactive desktop visualizer for:
- Sorting: Bubble, Selection, Insertion, Merge, Quick
- Graph traversal: BFS, DFS, Dijkstra

## Features
- Non-blocking, step-driven animation loop
- Dark modern UI (1000x700 window)
- Algorithm dropdown + Start/Pause/Reset controls
- Runtime speed slider
- Sorting color states:
  - Default: white
  - Comparing: yellow
  - Swapping/writing: red
  - Sorted: green
- Stats panel:
  - Sorting: comparisons, swaps/writes, steps
  - Graph: visited nodes, steps
- Dynamic complexity hint for selected algorithm

## Project Structure
```
dsa-visualizer/
  main.cpp
  CMakeLists.txt
  visualizer/
    SortingVisualizer.h
    SortingVisualizer.cpp
    GraphVisualizer.h
    GraphVisualizer.cpp
  algorithms/
    sorting_algorithms.h
    sorting_algorithms.cpp
    graph_algorithms.h
    graph_algorithms.cpp
  ui/
    Button.h
    Button.cpp
    Slider.h
    Slider.cpp
  utils/
    constants.h
    helpers.h
    helpers.cpp
  assets/
```

## Install SFML

### Windows (vcpkg recommended)
1. Install [vcpkg](https://github.com/microsoft/vcpkg)
2. Install SFML:
   ```powershell
   vcpkg install sfml
   ```
3. Configure CMake with vcpkg toolchain:
   ```powershell
   cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build build --config Release
   ```

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install -y libsfml-dev
cmake -S . -B build
cmake --build build -j
```

## Build and Run

### CMake (cross-platform)
```bash
cmake -S . -B build
cmake --build build
./build/DSAVisualizer
```

On Windows with multi-config generators:
```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\DSAVisualizer.exe
```

### g++ (manual example, Linux/macOS)
```bash
g++ -std=c++17 -O2 \
  main.cpp \
  visualizer/SortingVisualizer.cpp visualizer/GraphVisualizer.cpp \
  algorithms/sorting_algorithms.cpp algorithms/graph_algorithms.cpp \
  ui/Button.cpp ui/Slider.cpp utils/helpers.cpp \
  -lsfml-graphics -lsfml-window -lsfml-system \
  -o DSAVisualizer
./DSAVisualizer
```

## Notes
- Font loading order:
  1. `assets/Roboto-Regular.ttf`
  2. `assets/Inter-Regular.ttf`
  3. Windows system fonts (`Segoe UI`, `Arial`)
- If no font loads, the app exits and prints a clear message.

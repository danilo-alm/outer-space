# Outer Space

This is a simple SFML program that simulates stars moving toward the viewer with depth perception.

## Requirements
- C++17 or later
- CMake 3.28+

## Build and run
1. Configure project:
```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
```
This step will download SFML, so it may take a while depending on your connection.

2. Compile the project:
Before compiling, you may want to tweak some values, such as `max_framerate`, in `src/configuration.hpp`.
```sh
cmake --build build
```
The first build will also build SFML.

3. Run the project:
```sh
./build/bin/LearningSFML
```

## Controls
- Press `ESC` or `Q` to exit the program.


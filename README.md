# Outer Space

Here's a YouTube video showing the program:

[![](https://img.youtube.com/vi/gY3VdqAehVQ/0.jpg)](https://www.youtube.com/watch?v=gY3VdqAehVQ)
![](outer_space.mp4)

This is a simple SFML program that simulates stars moving toward the viewer with depth perception.

## Requirements
- C++17 or later
- CMake 3.28+

SFML will be automatically downloaded and built by CMake, so no manual installation is required.

## Build and run
1. Configure project:

This step will download SFML, so it may take a while depending on your connection.
```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

2. Compile the project:

Before compiling, you may want to tweak some values, such as `max_framerate`, in `src/configuration.hpp`. The first build will also build SFML.
```sh
cmake --build build
```

3. Run the project:
```sh
./build/bin/LearningSFML
```

## Controls
- Press `ESC` or `Q` to exit the program.


# CubeAscii

This is a C project to print a 3D rotating cube in the terminal using ASCII characters with each face having a different
color.

It uses ANSI escape codes to move the cursor and change the color of the text.
I used my header only library for this project, you can find
it at [cUnicodeLib](https://github.com/Im-Rises/cUnicodeLib/tree/main/cUnicodeLib) this library is used to print the
unicode characters and to change the color of the text (foreground and background).

> **Note**  
> This project is based on the work from `Servet Gulnaroglu` [here](https://www.youtube.com/watch?v=p09i_hoFdd0)

## Demo

https://github.com/Im-Rises/cubeAscii/assets/59691442/f5bd5eae-67f1-4b75-88f0-6140808ba2f8

## How to use

### Build

To build the project, you need to have `CMake` installed on your computer.  
Then, you can run the following commands:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run

To run the project, you can run the following command:

```bash
./cubeAscii
```

### Options

You can use the following options:

- `-h` : to display the usage
- `-c <count>` : to set the number of cubes to display (default: 1) choose between 1 and 3
- `-g` : to display in gray mode the cubes
- `-m` : to set the max rotation speed
- `-n` : to set the min rotation speed

You can combine the commands to set the number of cubes and to display in gray mode at the same time:

```bash
./cubeAscii -c 3 -g
```

This will display 3 cubes in gray mode.

## Github-Actions

[![flawfinder](https://github.com/Im-Rises/CubeAscii/actions/workflows/flawfinder.yml/badge.svg?branch=main)](https://github.com/Im-Rises/CubeAscii/actions/workflows/flawfinder.yml)
[![CMake](https://github.com/Im-Rises/CubeAscii/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Im-Rises/CubeAscii/actions/workflows/cmake.yml)
[![CodeQL](https://github.com/Im-Rises/CubeAscii/actions/workflows/codeql.yml/badge.svg?branch=main)](https://github.com/Im-Rises/CubeAscii/actions/workflows/codeql.yml)
[![Cpp Cmake Publish](https://github.com/Im-Rises/CubeAscii/actions/workflows/cpp-cmake-publish.yml/badge.svg?branch=main)](https://github.com/Im-Rises/CubeAscii/actions/workflows/cpp-cmake-publish.yml)

The project is set with a set of different scripts:

- flawfinder : to check for security flaws in the code
- CMake : to build the project for Windows, macOS and Linux
- CodeQl : to check for code quality and security
- Cpp Cmake Publish : to publish the project on GitHub

## Documentations

Wikipedia:  
<https://en.wikipedia.org/wiki/ANSI_escape_code>  
<https://en.wikipedia.org/wiki/Rotation_matrix>

## Contributors

Quentin MOREL :

- @Im-Rises
- <https://github.com/Im-Rises>

[![GitHub contributors](https://contrib.rocks/image?repo=Im-Rises/CubeAscii)](https://github.com/Im-Rises/CubeAscii/graphs/contributors)

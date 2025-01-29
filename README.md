# PathFinder

![CI workflow status](https://github.com/int2str/PathFinder/actions/workflows/main.yml/badge.svg)

This program parses a RiskyLab.com/tilemap compatible tilemap file and calculates
a path for a battle unit of a given color to the circular target of the same color.

![Battle units and targets](docs/units_and_targets.png)

## Dependencies

### testrunner - Unit test framework

[https://github.com/int2str/testrunner](https://github.com/int2str/testrunner)

PathFinder uses my 'TestRunner' unit test framework for some simple unit tests.
In production, much more mature testing frameworks such as Google's gtest or
Catch2 are available.

### jsonlib - JSON file parser

[https://github.com/int2str/jsonlib](https://github.com/int2str/jsonlib)

To parse tile maps, my own 'jsonlib' library is used. As with 'TestRunner' above,
more mature (better, faster, stronger) JSON parsers (such as nlohmann/json) are
available. 'jsonlib', as with 'TestRunner' were created recreationally.

### libfmt - C++ formatting library

While std::print / std::println have finally landed with C++23, support is
still not universal on all platforms. For simplicity, this project used
libfmt for now.

### SFML - Simple and Fast Multimedia Library

This library is used for animating the path finding results and render the
tilemap sprites.

## Building and running

When cloning the repository, make sure to specify the **--recursive**
parameter to make sure the included submodules are cloned as well.

Example:
`git clone --recorsive https://github.com/int2str/PathFinder.git`

### Build dependencies

The code is built using the [ninja-build](https://ninja-build.org/) system,
using the clang++ compiler. A recent (clang-19 in 2025) version of
the clang toolchain is required since the code is relying on various
C++23 language features.

The code requires libfmt and libsfml to be available on the host machine.

On Ubuntu this can be achieved by installing these packages:
`sudo apt install libfmt-dev libsfml-dev`

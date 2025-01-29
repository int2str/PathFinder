# PathFinder

Simple path finder challenge program.

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

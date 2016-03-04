
# m2etis #

Welcome to **m2etis**!

m2etis is a template based publish subscribe network middleware.

## Features ##

 * Compile time configuration of channels
 * QoS aware strategies

## Platforms ##

m2etis has been tested on:

 * Linux
 * Windows

## Requirements ##

m2etis has some dependencies as requirements. We use CMake (2.8+) for the build system and require a C++11 capable compiler such as g++ 4.7 or Visual Studio 13 (or higher). All other dependencies can be provided as system library or be built by yourself using the automated build scripts provided in the dependencies directory.

## Documentation ##

The doxygen documentation can be found on our homepage: http://clockwork-origins.de/en/m2etis/docu

## Building from source ##

To build m2etis from source you need cmake and a C++11 compiler. For a complete library setup just use

cmake .
make

Tests can be enabled using -DWITH_TESTING=ON. This requires gtest on your system (or you build it with the appropriate dependency build script in the dependencies directory).

## Contributing Code ##

We welcome patches. We will set up a Contributor Agreement soon to take care of the legal aspect of contributions.

# Building and Installation

## Prerequisites

Specialised software is required in order to compile this source code. If one or more prerequisites are not installed, the final executable may not build or lack certain features.

### CMake

Download [CMake](https://cmake.org/) from the website or via package manager.

- Arch Linux: `sudo pacman -Syu cmake`

### libsup

[libsup](https://github.com/igni-project/libsup) is not currently available on any major package repositories and must be built and installed from source.

1. Clone libsup with a Git client of your choice. With the [official git command-line client](https://git-scm.com/downloads/linux), this will be `git clone https://github.com/igni-project/libsup.git`.

2. Follow build and installation instructions outlined by libsup/INSTALL.md. 

## Initialisation

The repository itself, as cloned, is not quite ready for compilation. Autotools must download some additional files to function properly.

Execute the `./bootstrap` script in the repository root to bootstrap Autotools.

## Building

1. Create a new, empty 'build' folder.
2. Open CMake by clicking its icon or entering `cmake-gui` in a command prompt.
3. Set the source code directory to this repositories root folder.
4. Set the build directory to the 'build' folder created in step 1.
5. Click on the `Configure` button to configure project settings.
6. When done configuring, click `Generate`, then `Open Project`.
7. Build the project with the IDE of your choice.


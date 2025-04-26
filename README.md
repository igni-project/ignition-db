# ignition

SUP debugging tool

## Prerequisites

Specialised software is required in order to compile this source code. If one or more prerequisites are not installed, the final executable may not build or lack certain features.

### Autotools

[GNU Autotools](https://en.wikipedia.org/wiki/GNU_Autotools) is readily
available on most linux systems package managers.

- Arch Linux: `sudo pacman -Syu autoconf automake libtool`
- Debian and Ubuntu: `sudo apt-get install -y autotools-dev autoconf`
- Fedora: `sudo dnf install autoconf automake`

### libsup

[libsup](https://github.com/igni-project/libsup) is not currently available on any major package repositories and must be built and installed from source.

1. Clone libsup with a Git client of your choice. With the [official git command-line client](https://git-scm.com/downloads/linux), this will be `git clone https://github.com/igni-project/libsup.git`.

2. Follow build and installation instructions outlined by libsup/INSTALL.md. 


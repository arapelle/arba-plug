# `arba-plug`

A C++ library providing plugin classes.

# Install #
## Requirements ##

Binaries:

- A C++20 compiler (ex: g++-13)
- CMake 3.26 or later

Testing Libraries (optional):

- [Google Test](https://github.com/google/googletest) 1.13 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-plug --recurse-submodules
```

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/arba-plug
cmake -P cmake/scripts/quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -P cmake/scripts/quick_install.cmake -- TESTS BUILD Debug DIR /tmp/local
```

## Uninstall ##
There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.
```
cd /path/to/installed-arba-plug/
cmake -P uninstall.cmake
```

# How to use
## Example - Load a plugin
```c++
#include <arba/plug/plugin.hpp>
#include <iostream>

int main()
{
    return EXIT_SUCCESS;
}
```

# License

[MIT License](./LICENSE.md) © arba-plug

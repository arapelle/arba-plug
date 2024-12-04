# `arba-plug`

A C++ library providing plugin classes.

# Install #
## Requirements ##

Binaries:

- A C++20 compiler (ex: g++-14)
- CMake 3.26 or later

Testing Libraries (optional):

- [Google Test](https://github.com/google/googletest) 1.14 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-plug
```

## Use with `conan`

Create the conan package.
```
conan create . --build=missing -c
```
Add a requirement in your conanfile project file.
```python
    def requirements(self):
        self.requires("arba-plug/0.1.0")
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
#include <arba/plug/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    plug::plugin plugin(PLUGIN_PATH);
    auto generate_int = plugin.find_function_ptr<int (*)()>("generate_int");
    std::cout << generate_int() << std::endl;
    return EXIT_SUCCESS;
}
```

# License

[MIT License](./LICENSE.md) © arba-plug

Compiling and installing WLA DX
===============================

CMake
-----

CMake is the recommended way to compile WLA DX.

### Generating

While in the repository, do:

    mkdir build && cd build && cmake ..

Add `-DCMAKE_BUILD_TYPE=Debug` to `cmake ..` to make a debug build or use
`-DCMAKE_BUILD_TYPE=RelWithDebInfo` for a (almost) release build with debug
information.

Add `-DCMAKE_INSTALL_PREFIX=prefix` to set the install prefix to `prefix` when you
install it.

### Building

If you're using the `Makefile` generator (by default on Unixes), use
`cmake --build . -- -j4` to build with 4 threads for greater speed. Change the
4 to the number of cores your system has.

### Installing

And to finally installing it:

    cmake -P cmake_install.cmake

Add `-DCMAKE_INSTALL_PREFIX=prefix` before the `-P` to set the install prefix
to `prefix` when you install it.

The options `--config Release` and `-C Release` are only needed for generators
that have a config system, like Visual Studio.

### Available options

Either add `-Doption=value` to the `cmake ..` command or set it in the
`CMakeCache.txt` file.

* `CMAKE_BUILD_TYPE` (default: `Release`): Set the type of build, like `Debug`,
    `RelWithDebInfo` or `MinSizeRel`.
* `CMAKE_INSTALL_PREFIX` (default: depends on system): Set where to install
   the binaries and documentation.
* `GDB_DEBUGGING` (default: `OFF`): Whenever to use gdb debugging info, only
    for `Debug` with either GCC or Clang >= 3.8
* `GEN_DOC` (default: `ON` if `sphinx-build` is found, `OFF` otherwise):
    Whenever to build the documentation using sphinx
* `SPHINX_DIR` (default: empty): Path of the folder where `sphinx-build` is in.


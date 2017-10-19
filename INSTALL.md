Compiling and installing WLA DX
===============================

CMake
-----

CMake is the recommended way to compile and test WLA DX.

### Generating

While in the repository, do:

    mkdir build && cd build && cmake ..

Add `-DCMAKE_BUILD_TYPE=Debug` to `cmake ..` to make a debug build or use
`-DCMAKE_BUILD_TYPE=RelWithDebInfo` for a (almost) release build with debug
information.

### Building

If you're using the `Makefile` generator (by default on Unixes), use
`cmake --build . -- -j4` to build with 4 threads for greater speed. Change the
4 to the number of cores your system has.

### Testing

To test, call `ctest`:

    ctest . -C Release

### Installing

And to finally installing it:

    cmake -P cmake_install.cmake

The options `--config Release` and `-C Release` are only needed for generators
that have a config system, like Visual Studio.

### Avilable options

Either add `-Doption=value` to the `cmake ..` command or set it in the
`CMakeCache.txt` file.

* `CMAKE_BUILD_TYPE` (default: `Release`): Set the type of build, like `Debug`,
    `RelWithDebInfo` or `MinSizeRel`.
* `GDB_DEBUGGING` (default: `OFF`): Whenever to use gdb debugging info, only
    for `Debug` with either GCC or Clang >= 3.8
* `GEN_DOC` (default: `ON` if `sphinx-build` is found, `OFF` otherwise):
    Whenever to build the documentation using sphinx
* `SPHINX_DIR` (default: empty): Path of the folder where `sphinx-build` is in.



Legacy Makefiles
----------------

There are handmade makefiles for systems where CMake is not avilable.

While in the root of the repository, just run:

    make -f mk/Makefile.amiga


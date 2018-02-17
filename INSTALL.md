Compiling and installing WLA DX
===============================

CMake
-----

CMake is the recommended way to compile and test WLA DX.

### Generating

While in the repository, run:

    mkdir build && cd build && cmake ..

Add `-DCMAKE_BUILD_TYPE=Debug` to `cmake ..` to make a debug build or use
`-DCMAKE_BUILD_TYPE=RelWithDebInfo` for a (almost) release build with debug
information. The latter is the default one.

### Building

While in the `build` folder, run:

    cmake --build .

On generators with multiple configurations (like Visual Studio), append
`--config RelWithDebInfo` to build the `RelWithDebInfo` configuration.

If you're using the `Makefile` generator (by default on Unixes), use
`cmake --build . -- -j4` to build with 4 threads for greater speed. Change the
`4` to the number of cores your system has.

### Testing

To test, just call `ctest`.

On generators with multiple configurations (like Visual Studio), append
`-C RelWithDebInfo` to test the binaries of the `RelWithDebInfo` configuration.


### Installing

And to finally installing it:

    cmake -P cmake_install.cmake

### Avilable options

Either add `-Doption=value` to the `cmake ..` command (if generating a new
build), `cmake .` command (when modifying an existing build) or set it in the
`CMakeCache.txt` file.

* `CMAKE_BUILD_TYPE` (default: `RelWithDebInfo`): Set the type of build, like
    `Release`, `Debug`, `RelWithDebInfo` or `MinSizeRel`.
* `GDB_DEBUGGING` (default: `OFF`): Whenever to use gdb debugging info, only
    for `Debug` with either GCC or Clang ≥ 3.8
* `GEN_DOC` (default: `ON` if `sphinx-build` is found, `OFF` otherwise):
    Whenever to build the documentation using sphinx
* `SPHINX_DIR` (default: empty): Path of the folder where `sphinx-build` is in.



Legacy Makefiles
----------------

There are Makefiles for systems where CMake is not avilable.

While in the root of the repository, just run:

    make -f mk/Makefile

You can specify some additional parameters by appending the command with,
for example "CC=clang" to use clang as the compiler.

### Avilable options

* `CC` (default: `$CC`): C compiler
* `LD` (default: `$CC`): Linker
* `RM` (default: `rm -rf`): Command to delete stuff
* `CFLAGS` (default: `$CFLAGS`): C Compiler flags
* `LDFLAGS` (default: `$LDFLAGS`): Linker flags
* `DEBUGFLAGS` (default: `-DNDEBUG`): Set to nothing to disable debug

### Generating Makefile

The POSIX shell script `mk/generate_mk.sh` is a shell script that generates a
Makefile from a list of architectures that should be built.

wla-dx
======

WLA DX - Yet Another
GB-Z80/Z80/6502/65C02/6510/65816/HUC6280/SPC-700
Multi Platform Cross Assembler Package

WLA DX is a set of tools to assemble assembly files to object or library files
(`wla-CPU`) and linking them together (`wlalink`). WLA DX isn't bound to one
system (eg. NES or C64), but rather WLA DX allows you to define your own
memory map (that should be shared across all object and library files).

Supported architectures are:

* z80
* gb (z80-gb)
* 6502
* 65c02
* 6510
* 65816
* huc6580
* spc700



Installing
==========

Compiling
---------

See `INSTALL.md` for more detailed instructions.

You need:

* CMake (at least v2.8.9)
* C compiler
* Sphinx (min. v1.2.0, v1.6.0 recommended, only used to generate docs)
    * Sphinx version might be bumped up to 1.5.0 when cygwin updates their
      `python-sphinx` version.

    git clone https://github.com/vhelin/wla-dx && cd wla-dx
    mkdir build && cd build
    cmake ..
    cmake --build . --config RelWithDebInfo
    ctest -C RelWithDebInfo
    cmake -P cmake_install.cmake

The `ctest` command runs the tests, and the last `cmake -P` command installs
the binaries. The parameters `--config RelWithDebInfo` and `-C RelWithDebInfo`
can be omitted if you're not using a build system with multiple configurations
(like Ninja or Makefiles).

The compiled binaries are in the `binaries` folder (`bin` when installing) and
the compiled documentations are located in the `doc` folder.



Usage
=====

Use stones.



Contributing
============

See `Contribution.md` for information about the coding style and fixing bugs.

Bug Reporting
-------------

If you've found a bug, please submit an issue and an example project. You'll
find an template project to copy from in `tests/template/`. You'll find some
information of setting it up correctly in the `tests/README.md`.

You should be OK that your example project would then be included in the
repository as a test.

Community
---------

If GitHub isn't a community for you, you might want to visit an unofficial
IRC channel, such as `##wla-dx` on [freenode.net](https://freenode.net/)
([Webchat](https://webchat.freenode.net?channels=%23%23wla-dx)).


WLA DX (Wzonka-Lad Assembler Deluxe)
====================================

WLA DX - Yet Another
GB-Z80/Z80/Z80N/6502/65C02/65CE02/65816/68000/6800/6801/6809/8008/8080/HUC6280/SPC-700/SuperFX
Multi Platform Cross Assembler Package

Azure Pipelines CI:

* [![Build Status](https://dev.azure.com/villehelin0486/villehelin/_apis/build/status%2Fvhelin.wla-dx%20Linux%20Sanitizers?branchName=master)](https://dev.azure.com/villehelin0486/villehelin/_build/latest?definitionId=24&branchName=master) - Linux Sanitizers (ASan, UBSan, MSan)
* [![Build Status](https://dev.azure.com/villehelin0486/villehelin/_apis/build/status%2Fvhelin.wla-dx%20Windows%20MSVC?branchName=master)](https://dev.azure.com/villehelin0486/villehelin/_build/latest?definitionId=23&branchName=master) - Windows MSVC
* [![Build Status](https://dev.azure.com/villehelin0486/villehelin/_apis/build/status%2Fvhelin.wla-dx%20Windows%20Clang?branchName=master)](https://dev.azure.com/villehelin0486/villehelin/_build/latest?definitionId=22&branchName=master) - Windows Clang
* [![Build Status](https://dev.azure.com/villehelin0486/villehelin/_apis/build/status%2Fvhelin.wla-dx%20Windows%20XP%2032-bit?branchName=master)](https://dev.azure.com/villehelin0486/villehelin/_build/latest?definitionId=26&branchName=master) - Windows XP 32-bit
* [![Build Status](https://dev.azure.com/villehelin0486/villehelin/_apis/build/status%2Fvhelin.wla-dx%20macOS%20GCC?branchName=master)](https://dev.azure.com/villehelin0486/villehelin/_build/latest?definitionId=21&branchName=master) - macOS GCC
* [![Build Status](https://dev.azure.com/villehelin0486/villehelin/_apis/build/status%2Fvhelin.wla-dx%20macOS%20Clang?branchName=master)](https://dev.azure.com/villehelin0486/villehelin/_build/latest?definitionId=20&branchName=master) - macOS Clang
* [![Build Status](https://dev.azure.com/villehelin0486/villehelin/_apis/build/status%2Fvhelin.wla-dx%20Linux%20GCC?branchName=master)](https://dev.azure.com/villehelin0486/villehelin/_build/latest?definitionId=19&branchName=master) - Linux GCC
* [![Build Status](https://dev.azure.com/villehelin0486/villehelin/_apis/build/status%2Fvhelin.wla-dx%20Linux%20Clang?branchName=master)](https://dev.azure.com/villehelin0486/villehelin/_build/latest?definitionId=18&branchName=master) - Linux Clang

Documentation:

[![Documentation Status](https://readthedocs.org/projects/wla-dx/badge/?version=latest)](https://wla-dx.readthedocs.io/en/latest/?badge=latest) - Read the Docs

WLA DX is a set of tools to assemble assembly files to object or library files
(`wla-ARCH`) and linking them together (`wlalink`). WLA DX isn't bound to one
system (eg. NES or C64), but rather WLA DX allows you to define your own
memory map (that should be shared across all object and library files).

Supported architectures are:

* Z80
* Z80N
* GB (GB-Z80)
* 6502
* 65C02
* 65CE02
* 65816
* 68000
* 6800
* 6801
* 6809
* 8008
* 8080
* HuC6280
* SPC700
* SuperFX



Installing
==========

See `INSTALL` for more detailed instructions.

Compiling
---------

You need:

* CMake (at least v2.8.12)
* C compiler
* Sphinx (optional, min. v1.2.0, v1.6.0 recommended) (to generate docs)

1. `git clone https://github.com/vhelin/wla-dx # Clone the repository`
2. `cd wla-dx`
3. `mkdir build && cd build # Create building directory`
4. `cmake .. # Generate build system`
5. `cmake --build . --config Release # Build it`
6. `cmake -P cmake_install.cmake # Install it (optional)`

If you didn't install it, you can still access the binaries in the `binaries`
folder. It also contains the compiled documentation in the subfolder `doc`.

You can add `-DCMAKE_INSTALL_PREFIX=prefix` to Steps 4 and/or 6 (before the `-P`)
to set the prefix directory it'll install in.


Usage
=====

Use the [documentation](https://wla-dx.readthedocs.io/en/latest/) as a starting
point.


License
=======

WLA DX is GPL-2.0-or-later software. See the file LICENSE or https://spdx.org/licenses/GPL-2.0-or-later.html
for more information.

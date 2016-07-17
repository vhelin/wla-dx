## Documentation

This directory contains the documentation. It is built using
[Sphinx](http://www.sphinx-doc.org/), a originally python documentation
generator.

### Tools

This part was created using `sphinx-quickstart`. That tool creates a `Makefile`
and a Windows `make.bat`. They output to the `build` folder. This should be
used when testing, as it is way easier to run `make` than having to call CMake.
The contents of the `build`-folder is also ignored by git, so you don't have
to care about this folder for git.

When using CMake, we use `sphinx-build` directly to specify the build-folder
to be in the right build folder. This is preferred becuase of CMake.

### Guidelines

Look at the other sections and do it like them.


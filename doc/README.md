## Documentation

This directory contains the documentation. It is built using
[Sphinx](http://www.sphinx-doc.org/), a originally python documentation
generator.

### Generating

You can use CMake to generate the documentation. There is a target called
`doc`, which generates the documentation. (e. g. use `make doc`)
At the moment, only certain types can be used: Single HTML, Manpages and text.

You can also directly use `sphinx-build` directly. Use the documentation of
Sphnix for how to use it. (Quick reference: `sphnix-build -b TYPE . build`)

### Guidelines

Look at the other sections and do it like them.


list(APPEND DEFS "NAME=kullervo")

wla(LIBRARY
    ARCH "${ARCH}"
    SOURCE "${SOURCE_DIR}/test.s"
    OUTPUT "${BINARY_DIR}/test.lib"
    VERBOSE)


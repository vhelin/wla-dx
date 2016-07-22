# Documentation CMakeLists file

project(DOCS)

option(GEN_DOC "Generate Sphinx documentation" ON)
option(GEN_DOC_MAN "Generate Manpages" ON)
option(GEN_DOC_SINGLEHTML "Generate single HTML documentation" ON)
option(GEN_DOC_TEXT "Generate text documentation" ON)

set(DOC_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/doc")
set(DOC_SRC "${CMAKE_CURRENT_SOURCE_DIR}")
set(DOC_DEPS )

if (!GEN_DOC)
    return()
endif (!GEN_DOC)

# Find sphnix-build
# eb2.co/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
find_program(SPHINX_EXECUTABLE
    NAMES sphinx-build
    HINTS $ENV{SPHINX_DIR}
    PATH_SUFFIXES bin
    DOC "Sphinx documentation generator"
)

if (SPHINX_EXECUTABLE STREQUAL "SPHINX_EXECUTABLE-NOTFOUND")
    message(WARNING "Sphinx not found")
    return()
endif (SPHINX_EXECUTABLE STREQUAL "SPHINX_EXECUTABLE-NOTFOUND")

if (GEN_DOC_MAN)
    set(DOC_MAN_CPUFILES "${DOC_OUT}/wla-gb.1" "${DOC_OUT}/wla-65c02.1"
        "${DOC_OUT}/wla-6502.1" "${DOC_OUT}/wla-6510.1" "${DOC_OUT}/wla-65816.1"
        "${DOC_OUT}/wla-huc6280.1" "${DOC_OUT}/wla-spc700.1"
        "${DOC_OUT}/wla-z80.1"
    )
    add_custom_command(
        OUTPUT "${DOC_OUT}/wla-dx.7" "${DOC_OUT}/wlalink.1" "${DOC_OUT}/wlad.1"
            "${DOC_OUT}/wlab.1" ${DOC_MAN_CPUFILES}
        COMMAND ${SPHINX_EXECUTABLE} -q -b man "${DOC_SRC}" man
        COMMAND ${CMAKE_COMMAND} -E copy_directory man/ "${DOC_OUT}"
        COMMENT "Generate manpages"
    )
    list(APPEND DOC_DEPS "${DOC_OUT}/wla-dx.7" "${DOC_OUT}/wlalink.1"
        "${DOC_OUT}/wlad.1" "${DOC_OUT}/wlab.1" ${DOC_MAN_CPUFILES})
    
    install(FILES "${DOC_OUT}/wla-dx.7" DESTINATION man/man7)
    install(FILES "${DOC_OUT}/wlalink.1" "${DOC_OUT}/wlad.1"
        "${DOC_OUT}/wlab.1" ${DOC_MAN_CPUFILES} DESTINATION man/man1)
endif (GEN_DOC_MAN)

if (GEN_DOC_SINGLEHTML)
    add_custom_command(
        OUTPUT "${DOC_OUT}/wla-dx.html"
        COMMAND ${SPHINX_EXECUTABLE} -q -b singlehtml "${DOC_SRC}" singlehtml
        COMMAND ${CMAKE_COMMAND} -E copy singlehtml/index.html
            "${DOC_OUT}/wla-dx.html"
        COMMAND ${CMAKE_COMMAND} -E copy_directory singlehtml/_static/
            "${DOC_OUT}/_static/"
        COMMENT "Generate single HTML page documentation"
    )
    list(APPEND DOC_DEPS "${DOC_OUT}/wla-dx.html")
    
    install(FILES "${DOC_OUT}/wla-dx.html" DESTINATION share/doc/wla-dx)
    install(DIRECTORY "${DOC_OUT}/_static" DESTINATION share/doc/wla-dx)
endif (GEN_DOC_SINGLEHTML)

if (GEN_DOC_TEXT)
    add_custom_command(
        OUTPUT "${DOC_OUT}/wla-dx.txt"
        COMMAND ${SPHINX_EXECUTABLE} -q -b text "${DOC_SRC}" text
        COMMAND ${CMAKE_COMMAND} -E copy text/index.txt
            "${DOC_OUT}/wla-dx.txt"
        COMMENT "Generate simple text documentation"
    )
    list(APPEND DOC_DEPS "${DOC_OUT}/wla-dx.txt")
    
    install(FILES "${DOC_OUT}/wla-dx.txt" DESTINATION share/doc/wla-dx)
endif (GEN_DOC_TEXT)

add_custom_target(doc ALL DEPENDS ${DOC_DEPS})

